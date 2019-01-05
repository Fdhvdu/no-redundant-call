#include <iostream>
#include <optional>
#include <vector>
using namespace std;

namespace
{
	template<class T>
	bool pop_if_exist(vector<T> &vec, T &item)
	{
		if (vec.size() == 0)
			return false;
		item = move(vec.back());
		vec.pop_back();
		return true;
	}

	template<class T>
	std::optional<T> pop_if_exist(vector<T> &vec)
	{
		if (vec.size() == 0)
			return nullopt;
		auto opt(make_optional(move(vec.back())));
		vec.pop_back();
		return opt;
	}
}

int main()
{
	vector<int> vec1({3,2,1});
	for (int item; pop_if_exist(vec1, item);)
		cout << item << endl;

	vector<int> vec2({3,2,1});
	while (auto item = pop_if_exist(vec2))
		cout << item.value() << endl;
}