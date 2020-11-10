#include <bits/stdc++.h>

std::mt19937 rng(std::random_device{}());

void uniform(float b[], int n) {
	float a[n];
	std::copy(b, b + n, a);

	float sum = std::accumulate(a, a + n, 0.f);
	for (int i = 0; i < n; ++i)
		a[i] /= sum;

	std::sort(a, a + n, std::greater<>());

	std::cout << "uniform:\n";
	for (int i = 0; i < n; ++i)
		std::cout << a[i] << std::endl;
	std::cout << std::endl;
}

void sigmoid(float b[], int n) {
	float a[n];
	std::copy(b, b + n, a);

	float sum = 0.f;
	for (int i = 0; i < n; ++i)
		sum += std::exp(a[i]);
	for (int i = 0; i < n; ++i)
		a[i] = std::exp(a[i]) / sum;

	std::sort(a, a + n, std::greater<>());
	
	std::cout << "sigmoid\n";
	for (int i = 0; i < n; ++i)
		std::cout << a[i] << std::endl;
	std::cout << std::endl;
}

int main() {
	int n = 10;
	float a[n];
	for (int i = 0; i < n; ++i)
		a[i] = std::uniform_real_distribution<float>{0, 1}(rng);

	std::cout << "array a:\n";
	for (int i = 0; i < n; ++i)
		std::cout << a[i] << std::endl;
	std::cout << std::endl;

	uniform(a, n);
	sigmoid(a, n);

	return 0;
}
