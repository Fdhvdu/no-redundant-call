cmake_minimum_required(VERSION 3.13.0)

option(ENABLE_TESTING "enable test" ON)

set(CMAKE_CXX_STANDARD 17)

if(${ENABLE_TESTING})
	enable_testing()

	include(ExternalProject)

	ExternalProject_Add(googletest
		PREFIX googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.8.1
		GIT_SHALLOW TRUE
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_GMOCK=OFF
	)

	ExternalProject_Get_Property(googletest INSTALL_DIR)

	add_executable(gtest test/test.cpp)

	target_include_directories(gtest PRIVATE include/)
	target_include_directories(gtest PRIVATE ${INSTALL_DIR}/include/)

	target_link_libraries(gtest ${INSTALL_DIR}/lib64/libgtest.a)
	target_link_libraries(gtest ${INSTALL_DIR}/lib64/libgtest_main.a)
	target_link_libraries(gtest pthread)

	include(GoogleTest)
	gtest_discover_tests(gtest)
endif()
