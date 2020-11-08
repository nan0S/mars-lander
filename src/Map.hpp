#ifndef MAP_HPP
#define MAP_HPP

#include "Common.hpp"
#include "Agent.hpp"

#include <string>
#include <vector>

class Map {
public:
	static void load();
	static void init(Agent& agent);
	static bool isCrashed(const Agent& agent);
	static float evaluate(const Agent& agent);

private:
	static void show();

public:
	static int pointCount;
	static std::vector<Point> points;
	
	static constexpr int LEFT_BORDER = 0;
	static constexpr int RIGHT_BORDER = 6999;
	static constexpr int TOP_BORDER = 3000;
	static constexpr int BOT_BORDER = 0;

private:
	static const std::string mapNames[];
	static const int mapCount;
	static std::string currentMapName;

	static int landLeft, landRight;
	static int landMiddle;
	static int landGround;
	static Vector initialPos;
	static Vector initialVel;
	static int initialFuel;
	static int initialAngle;
	static int initialThrust;
};

#endif /* MAP_HPP */