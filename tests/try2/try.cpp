#include <bits/stdc++.h>

constexpr float DEGTORAD = M_PI / 180;
constexpr float G = 3.711;

int main() {
	// for (int j = 0; j <= 4; ++j) {
	// 	std::cout << "{\n";
	// 	for (int i = -90, prt = 0; i <= 90; ++i, ++prt ) {
	// 		std::cout << j * sinf(DEGTORAD * i) << ", ";
	// 		if (prt == 5) {
	//  			std::cout << std::endl;
	// 			prt = 0;
	// 		}
	// 	}
	// 	std::cout << "},\n";
	// }

	for (int j = 0; j <= 4; ++j) {
		std::cout << "{\n";
		for (int i = -90, prt = 0; i <= 90; ++i, ++prt ) {
			std::cout << j * cosf(DEGTORAD * i) - G << ", ";
			if (prt == 5) {
	 			std::cout << std::endl;
				prt = 0;
			}
		}
		std::cout << "},\n";
	}

	return 0;
}