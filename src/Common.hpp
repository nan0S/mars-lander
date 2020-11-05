#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <iostream>

#define x first
#define y second

typedef std::pair<int, int> Point;

void errorExit(const std::string& msg);

std::ostream& operator<<(std::ostream& out, const Point& p);

#endif /* COMMON_HPP */