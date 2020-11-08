#include <bits/stdc++.h>

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p) {
	return out << "(" << p.first << ", " << p.second << ")";
}

int main() {
	std::pair<int, int> p{1, 2};
	std::cout << p;

	std::pair<float, int> p2{1.3, 4};
	std::cout << p2;

	return 0;
}