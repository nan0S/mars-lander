#include <bits/stdc++.h>

namespace Random {
	// std::mt19937 rng(std::random_device{}());
	std::mt19937 rng(32132131);

	template<typename T>
	using T_Int = std::enable_if_t<
		std::is_integral_v<T>, T>;

	template<typename T>
	using NT_Int = std::enable_if_t<
		!std::is_integral_v<T>, T>;

	template<typename T>
	using dist_t = std::conditional_t<
		std::is_integral_v<T>,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>
	>;

	template<typename T>
	NT_Int<T> rand() {
		return dist_t<T>{0, 1}(rng);
	}

	template<typename T>
	T_Int<T> rand(T n) {
		return dist_t<T>{0, n-1}(rng);
	}

	template<typename T>
	NT_Int<T> rand(T x) {
		return dist_t<T>{0, x}(rng);
	}

	template<typename T>
	T rand(T a, T b) {
		return dist_t<T>{a, b}(rng);
	}
}

const int T = 10;

void randTests() {
	for (int i = 0; i < T; ++i) {
		auto f = Random::rand<float>();
		std::cout << f << std::endl;
		assert(0 <= f && f <= 1);
	}

	for (int i = 0; i < T; ++i) {
		auto d = Random::rand<double>();
		std::cout << d << std::endl;
		assert(0 <= d && d <= 1);
	}

	for (int i = 0; i < T; ++i) {
		auto a = Random::rand(10);
		std::cout << a << std::endl;
		assert(0 <= a && a < 10);
	}

	for (int i = 0; i < T; ++i) {
		auto a = Random::rand(0.5f);
		std::cout << a << std::endl;
		assert(0 <= a && a <= 0.5f);
	}

	for (int i = 0; i < T; ++i) {
		auto a = Random::rand(10, 15);
		std::cout << a << std::endl;
		assert(10 <= a && a <= 15);
	}

	for (int i = 0; i < T; ++i) {
		auto a = Random::rand(1.5f, 2.3f);
		std::cout << a << std::endl;
		assert(1.5f <= a && a <= 2.3f);
	}
}

int main() {
	randTests();

	return 0;
}