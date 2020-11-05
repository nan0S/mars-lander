#include "Common.hpp"

#include <iostream>

void errorExit(const std::string& msg) {
	std::cerr << "ERROR: " << msg << '\n';
	exit(EXIT_FAILURE);
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
	return out << "(" << p.x << ", " << p.y << ")";
}