#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

struct Timer {
	string name;
	time_point<high_resolution_clock> start;

	Timer(const string& name) : 
		name(name), start(high_resolution_clock::now()) {
	}

	~Timer() {
		cout << name << ": " << duration<double>(high_resolution_clock::now() - start).count() << endl;
	}
};

const int N = 180 + 7;
float coss[N];
float sinn[N];
vector<int> v;

void f1() {
	Timer t("f1");
	int s = int(v.size());
	float sum = 0;
	for (int i = 0; i < s; ++i)
		sum += coss[v[i]];
	cout << sum << endl;
}

void f2() {
	Timer t("f2");
	int s = int(v.size());
	float sum = 0;
	for (int i = 0; i < s; ++i)
		sum += cosf(v[i]);
	cout << sum << endl;
}

void measure() {
	v.clear();
	const int T = 10000000;
	v.reserve(T);
	mt19937 gen(random_device{}());
	uniform_int_distribution<int> dist(0, 180);
	for (int i = 0; i < T; ++i) {
		int x = dist(gen);
		v.emplace_back(x);
	}
	if (dist(gen) >= 90)
		cout << "f1, f2\n", f1(), f2();
	else
		cout << "f2, f1\n", f2(), f1();
}

int main() {
	for (int i = 0; i <= 180; ++i)
		coss[i] = std::cos(float(i));
	for (int i = 0; i <= 180; ++i)
		sinn[i] = std::cos(float(i));

	measure();

	return 0;
}