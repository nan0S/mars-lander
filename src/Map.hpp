#ifndef MAP_HPP
#define MAP_HPP

#include "Common.hpp"

#include <string>
#include <vector>

class Map {
public:
	static const std::string mapNames[];
	static const int mapCount;
	static std::string currentMapName;

	static int n;
	static std::vector<Point> points;
	static int landLeft, landRight;
	static int landGround;
	static Point initialPos;
	static int initialFuel;
	static int initialRot;
	static int initialThrust;

	static void loadMap(int mapNr);
	static void showMap();


};

#endif /* MAP_HPP */