#include "Map.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

const std::string Map::mapNames[] = {
	"easy_on_the_right",
	"initial_speed_correct_side",
	"initial_speed_wrong_side",
	"deep_canyon",
	"high_ground",
};

const int Map::mapCount = sizeof(mapNames) / sizeof(mapNames[0]);
std::string Map::currentMapName;

const std::string testPath = "tests/";

int Map::n;
std::vector<Point> Map::points;
int Map::landLeft;
int Map::landRight;
int Map::landGround;
Point Map::initialPos;
int Map::initialFuel;
int Map::initialRot;
int Map::initialThrust;

void Map::loadMap(int mapNr) {
	assert(1 <= mapNr && mapNr <= mapCount);

	currentMapName = mapNames[mapNr - 1];
	std::string path = testPath + currentMapName + ".txt";

	std::ifstream ifs(path);
	assert(ifs.is_open());

	ifs >> n;
	for (int i = 0; i < n; ++i) {
		int x, y;
		ifs >> x >> y;
		points.emplace_back(x, y);
	}
	
	ifs >> landLeft >> landRight;
	ifs >> landGround;
	ifs >> initialPos.x >> initialPos.y;
	ifs >> initialFuel >> initialRot >> initialThrust;
}

void Map::showMap() {
	std::cout << "\nMap name: " << currentMapName << "\n";
	std::cout << "\nNumber of points: " << n << "\n";
	for (const auto& p : points)
		std::cout << p << "\n";
	std::cout << "\n";

	std::cout << "Landing zone: " << Point(landLeft, landGround) << " " << Point(landRight, landGround) << "\n\n";
	std::cout << "Initial position: " << initialPos << "\n";
	std::cout << "Initial fuel: " << initialFuel << "\n";
	std::cout << "Initial rotation: " << initialRot << "\n";
	std::cout << "Initial thrust: " << initialThrust << "\n\n";
}