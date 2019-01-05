# No redundant call
Image a function will return same output if input are same. To reduce redundant call, you may want a class can memorize previous return value and return it to you. [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) is used for this purpose.

[No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) will compare arguments you pass to all previous function calls. If [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) can find an exactly match, [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) will return the previous return value (if it is not void type). Otherwise, [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) will invoke the function and store the return value so that following function calls does not have to invoke the function again (if arguments are same).

**If your function has *side effect*, [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) cannot help you.**

# Usage
See [Test](https://github.com/Fdhvdu/no-redundant-call/blob/master/test/test.cpp).

# Warnings
In a function below,
```cpp
void store_to_arg(int &i)
{
	i = 10;
}
```
[No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) cannot know reference type is used for output (`int &i`). As a result, the following code will make [No_redundant_call](https://github.com/Fdhvdu/no-redundant-call/blob/master/include/No_redundant_call.hpp) strange but correct.
```cpp
nTool::No_redundant_call no_redundant;

int i(0);
no_redundant.invoke(store_to_arg, i);
cout << i << endl;	// 10

i = 0;
no_redundant.invoke(store_to_arg, i);
cout << i << endl;	// 0, seriously?

i = 1;
no_redundant.invoke(store_to_arg, i);
cout << i << endl;	// 10
```
I also demonstrate another solution ([sample](https://github.com/Fdhvdu/no-redundant-call/blob/master/sample/sample.cpp)). Hope it can help you.
