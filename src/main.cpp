#include <vector>
#include <type_traits>
#include <iostream>
#include <functional>
#include <string>
#include <concepts>

using namespace std;

template <class T>
void print_arr(const vector<T>& _arr) {
	cout << "[";
	for (size_t i = 0; i < _arr.size(); ++i) {
		cout << _arr[i];
		cout << ((i == _arr.size() - 1) ? "]" : ",");
	}
	cout << endl;
}

template<class F, class ...Args>
concept is_predicate = requires(F _predicate, Args... _args) {
	{ invoke(_predicate, _args...) } -> same_as<bool>;
};

template <class T>
vector<T> filter(const vector<T>& _arr, std::function<bool(T)> _pred) {
	vector<T> ret;
	for (auto& x : _arr) {
		if (_pred(x)) {
			ret.push_back(x);
		}
	}
	return ret;
}

template <class T, class Predicate>
enable_if_t<is_same_v<invoke_result_t<Predicate, T>, bool>, vector<T>> filter_sfinae(const vector<int>& _arr, Predicate _pred) {
	vector<T> ret;
	for (auto i : _arr) {
		if (_pred(i)) {
			ret.push_back(i);
		}
	}
	return ret;
}

template <class T, class Predicate>
vector<T> filter_concept(const vector<int>& _arr, Predicate _pred) requires is_predicate<Predicate, T> {
	vector<T> ret;
	for (auto i : _arr) {
		if (_pred(i)) {
			ret.push_back(i);
		}
	}
	return ret;
}

auto is_odd = [](int _x) { return _x % 2 == 1; };
auto is_even = [](int _x) { return _x % 2 == 0; };
auto mult2 = [](int _x) { return _x + _x; };

int main() {
	vector<int> arr{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	print_arr(arr);

	{
		cout << "Relaxed Filter:" << endl;
		
		auto even = filter<int>(arr, is_even);
		print_arr(even);

		auto odd = filter<int>(arr, is_odd);
		print_arr(odd);

		auto wrong = filter<int>(arr, mult2);
		print_arr(wrong);

		cout << endl;
	}

	{
		cout << "SFINAE Filter:" << endl;

		auto even = filter_sfinae<int>(arr, is_even);
		print_arr(even);

		auto odd = filter_sfinae<int>(arr, is_odd);
		print_arr(odd);

		// Failed to specialize function template 'enable_if<std::is_same_v<_Select_invoke_traits<Predicate,T>::type,bool>,std::vector<T,std::allocator<_Ty>>>::type filter_sfinae(const std::vector<int,std::allocator<int>> &,Predicate)'
		// vector<int> error = filter_sfinae<int>(arr, mult2);

		cout << endl;
	}

	{
		cout << "Concept Filter:" << endl;
		
		auto even = filter_concept<int>(arr, is_even);
		print_arr(even);

		auto odd = filter_concept<int>(arr, is_odd);
		print_arr(odd);

		/* the associated constraints are not satisfied
		the concept 'is_predicate<<lambda>@mult2,int>' evaluated to false
		the concept 'std::same_as<int,bool>' evaluated to false */
		// vector<int> error = filter_concept<int>(arr, mult2);
		
		cout << endl;
	}

	return 0;
}