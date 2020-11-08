#include <bits/stdc++.h>

std::mt19937 rng(std::random_device{}());

void rand_permutation(int* begin, int* end) {
	std::iota(begin, end, 0);
	std::shuffle(begin, end, rng);
}

void print(int* begin, int* end) {
	while (begin != end)
		std::cout << *begin++ << " ";
	std::cout << std::endl;
}

int main() {
	const int n = 10;
	int a[n];
	rand_permutation(a, a + n);

	print(a, a + n);

	int s = std::uniform_int_distribution<int>(0, n)(rng);
	std::partial_sort(a, a + s, a + n, std::greater<int>());

	std::cout << s << std::endl;
	print(a, a + n);

	return 0;
}