#include <bits/stdc++.h>

template<typename T>
T clamp(T v, const T& a, const T& b) {
	return std::min(std::max(v, a), b);
}

const int T = 1000;

void clampTests() {
	assert(clamp(5, 1, 6) == 5);
	assert(clamp(10, 1, 5) == 5);
	assert(clamp(10, 11, 14) == 11);
	assert(clamp(1.0, 1.2, 1.4) == 1.2);
	assert(clamp(132.33, 100.0, 203.0) == 132.33);
}

int main() {
	clampTests();

	return 0;
}