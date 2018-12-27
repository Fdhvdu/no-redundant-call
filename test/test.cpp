#include <memory>
#include "gtest/gtest.h"
#include "No_redundant_call.hpp"

namespace
{
	struct Test1
	{
		int id = 0;
		int cnt = 0;
		void test()
		{
			++cnt;
		}
	};

	bool operator==(Test1 lhs, Test1 rhs) noexcept
	{
		return lhs.id == rhs.id;
	}

	TEST(FunctionObject, MemberFunction1)
	{
		nTool::No_redundant_call reduce;
		Test1 test;
		reduce.invoke(&Test1::test, test);
		EXPECT_EQ(1, test.cnt);
		reduce.invoke(&Test1::test, test);
		EXPECT_EQ(1, test.cnt);
	}

	struct Test2
	{
		int id = 0;
		int cnt = 0;
		void test(int)
		{
			++cnt;
		}
	};

	bool operator==(Test2 lhs, Test2 rhs) noexcept
	{
		return lhs.id == rhs.id;
	}

	TEST(FunctionObject, MemberFunction2)
	{
		nTool::No_redundant_call reduce;
		Test2 test;
		reduce.invoke(&Test2::test, test, 0);
		EXPECT_EQ(1, test.cnt);
		reduce.invoke(&Test2::test, test, 1);
		EXPECT_EQ(2, test.cnt);
		reduce.invoke(&Test2::test, test, 0);
		EXPECT_EQ(2, test.cnt);
		reduce.invoke(&Test2::test, test, 2);
		EXPECT_EQ(3, test.cnt);
		reduce.invoke(&Test2::test, test, 1);
		EXPECT_EQ(3, test.cnt);
	}

	struct Test3
	{
		int id = 0;
		int cnt = 0;
		int test(int, double)
		{
			return ++cnt;
		}
	};

	bool operator==(Test3 lhs, Test3 rhs) noexcept
	{
		return lhs.id == rhs.id;
	}

	TEST(FunctionObject, MemberFunction3)
	{
		nTool::No_redundant_call reduce;
		Test3 test;
		EXPECT_EQ(1, reduce.invoke(&Test3::test, test, 0, 0));
		EXPECT_EQ(1, test.cnt);
		EXPECT_EQ(2, reduce.invoke(&Test3::test, test, 0, 1));
		EXPECT_EQ(2, test.cnt);
		EXPECT_EQ(3, reduce.invoke(&Test3::test, test, 1, 0));
		EXPECT_EQ(3, test.cnt);
		EXPECT_EQ(2, reduce.invoke(&Test3::test, test, 0, 1));
		EXPECT_EQ(3, test.cnt);
		EXPECT_EQ(1, reduce.invoke(&Test3::test, test, 0, 0));
		EXPECT_EQ(3, test.cnt);
	}

	struct Test4
	{
		int id = 0;
		int cnt = 0;
		int test(int &i)
		{
			++cnt;
			return i;
		}
	};

	bool operator==(Test4 lhs, Test4 rhs) noexcept
	{
		return lhs.id == rhs.id;
	}

	TEST(FunctionObject, MemberFunction4)
	{
		nTool::No_redundant_call reduce;
		Test4 test;
		int temp(0);
		EXPECT_EQ(0, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(1, test.cnt);
		temp = 1;
		EXPECT_EQ(1, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(2, test.cnt);
		temp = 2;
		EXPECT_EQ(2, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(3, test.cnt);
		temp = 1;
		EXPECT_EQ(1, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(3, test.cnt);
		temp = 2;
		EXPECT_EQ(2, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(3, test.cnt);
		temp = 0;
		EXPECT_EQ(0, reduce.invoke(&Test4::test, test, temp));
		EXPECT_EQ(3, test.cnt);
	}

	struct Test5
	{
		int id = 0;
		int cnt = 0;
		int operator()(int)
		{
			return ++cnt;
		}
	};

	bool operator==(Test5 lhs, Test5 rhs) noexcept
	{
		return lhs.id == rhs.id;
	}

	TEST(FunctionObject, MemberFunction5)
	{
		nTool::No_redundant_call reduce;
		Test5 test;
		EXPECT_EQ(1, reduce.invoke(test, 0));
		EXPECT_EQ(1, test.cnt);
		EXPECT_EQ(2, reduce.invoke(test, 1));
		EXPECT_EQ(2, test.cnt);
		EXPECT_EQ(1, reduce.invoke(test, 0));
		EXPECT_EQ(2, test.cnt);
		EXPECT_EQ(3, reduce.invoke(test, 2));
		EXPECT_EQ(3, test.cnt);
		EXPECT_EQ(2, reduce.invoke(test, 1));
		EXPECT_EQ(3, test.cnt);
	}
}
