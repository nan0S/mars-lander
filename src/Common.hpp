#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef LOCAL
#define NDEBUG
#endif

#if defined(LOCAL) && !defined(NDEBUG)
#define DEBUG
#endif

#include <utility>
#include <iostream>
#include <random>
#include <chrono>

#define x first
#define y second

typedef std::pair<int, int> Point;
typedef std::pair<float, float> Vector;

#ifdef DEBUG
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p) {
	return out << "(" << p.x << " , " << p.y << ")";
}
#endif

namespace Random {
	extern std::mt19937 rng;

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

template<typename T>
T clamp(T v, const T& a, const T& b) {
	return std::min(std::max(v, a), b);
}

extern const float ssin[];
extern const float scos[];

inline float sin(int x) {
	return ssin[x + 90];
}

inline float cos(int x) {
	return scos[x + 90];
}

template<typename T1, typename T2>
inline bool orientation(const std::pair<T1, T1>& p1, const std::pair<T1, T1>& p2, const std::pair<T2, T2>& q) {
	return (q.x - p1.x) * (p2.y - p1.y) - (q.y - p1.y) * (p2.x - p1.x) > 0;
}

inline bool collide(const Vector& p1, const Vector& p2, const Point& q1, const Point& q2) {
	bool o1 = orientation(p1, p2, q1);
	bool o2 = orientation(p1, p2, q2);
	bool o3 = orientation(q1, q2, p1);
	bool o4 = orientation(q1, q2, p2);
	return o1 != o2 && o3 != o4;
}

class Timer {
public:
	Timer(float givenTime);

	inline bool isTimeLeft() {
		auto end = std::chrono::high_resolution_clock::now(); 
		return std::chrono::duration<float>(end - start).count() * 1000 < givenTime;
	}

private:
	float givenTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#ifdef DEBUG

template<typename T>
void __debug(const char* s, const T& x) {
	std::cerr << s << " = " << x << " ";
}

template<typename T, typename... Args>
void __debug(const char* s, const T& x, const Args&... rest)
{
	int bracket = 0;
	char c;
	while ((c = *s) != ',' || bracket)
	{
		std::cerr << *s++;
		switch (c)
		{
			case '(':
			case '{':
			case '[':
				++bracket;
				break;
			case ')':
			case '}':
			case ']':
				--bracket;
		}
	}
	std::cerr << " = ";
	std::cerr << x << ",";
	__debug(s + 1, rest...);

}

template<typename... Args>
void _debug(const char* s, const Args&... rest) {
	std::cerr << "[ ";
	__debug(s, rest...);
	std::cerr << "]" << std::endl;
}

#define debug(...) _debug(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) 13
#endif


#endif /* COMMON_HPP */
