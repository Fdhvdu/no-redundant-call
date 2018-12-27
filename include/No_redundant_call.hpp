#ifndef NO_REDUNDANT_CALL_HPP
#define NO_REDUNDANT_CALL_HPP
#include <any>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace nTool
{
	// notice
	// 1. all parameters will be stored into std::any (even parameters are reference)
	// 2. return type of function cannot be reference
	// 3. require C++17
	class No_redundant_call
	{
		struct Function
		{
			std::any ret;
			std::any func;
			std::vector<std::any> param;
			template<class Func, class ... Args>
			Function(Func &&f, Args &&...args)
				:func(f)
			{
				param.reserve(sizeof...(Args));
				(param.push_back(args), ...);	// copy
				set_and_invoke_(std::is_void<std::invoke_result_t<Func, Args...>>{}, f, std::forward<decltype(args)>(args)...);
			}
			template<class Func, class ... Args>
			bool equal(Func &&f, Args &&...args) const
			{
				if (func.type() != std::make_any<decltype(f)>(f).type())
					return false;
				if constexpr (sizeof...(Args) == 0)
					return true;
				else
				{
					bool equal(true);
					for (std::size_t i(0); i != param.size(); ++i)
						if (!equal_(param[i], i, std::forward<decltype(args)>(args)...))
						{
							equal = false;
							break;
						}
					return equal;
				}
			}
		private:
			template<class Arg, class ... Args>
			static bool equal_(const std::any &a, const std::size_t n, Arg &&arg, Args &&...args)
			{
				using namespace std;
				if (n == 0)
					return any_cast<add_const_t<remove_reference_t<decltype(arg)>>>(a) == arg;
				if constexpr (sizeof...(Args) != 0)
					return equal_(a, n - 1, forward<decltype(args)>(args)...);
				return false;
			}
			template<class Func, class ... Args>
			inline void set_and_invoke_(std::true_type is_void, Func &&f, Args &&...args)
				noexcept(noexcept(std::invoke(f, std::forward<decltype(args)>(args)...)))
			{
				std::invoke(f, std::forward<decltype(args)>(args)...);
			}
			template<class Func, class ... Args>
			inline void set_and_invoke_(std::false_type not_void, Func &&f, Args &&...args)
				noexcept(
					noexcept(std::invoke(f, std::forward<decltype(args)>(args)...))
					&& noexcept(std::is_nothrow_move_constructible_v<std::invoke_result_t<Func, Args...>>)
				)
			{
				ret = std::invoke(f, std::forward<decltype(args)>(args)...);
			}
		};
		std::vector<Function> history_;
	public:
		template<class Func, class ... Args>
		auto invoke(Func &&f, Args &&...args)
		{
			using namespace std;
			using return_type = invoke_result_t<Func, Args...>;
			size_t i(0);
			for (; i != history_.size(); ++i)
				if (history_[i].equal(f, forward<decltype(args)>(args)...))
					break;
			if (i == history_.size())	// not found
				history_.emplace_back(f, forward<decltype(args)>(args)...);
			if constexpr (negation_v<is_void<return_type>>)
				return any_cast<remove_reference_t<return_type>>(history_[i].ret);
		}
	};
}

#endif
