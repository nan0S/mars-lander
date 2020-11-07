#ifndef MAP_HPP
#define MAP_HPP

#include "Common.hpp"
#include "Agent.hpp"

#include <string>
#include <vector>

enum class State {
	Good = 0,
	Fuel,
	Crash
};

std::ostream& operator<<(std::ostream& out, const State& state);

class Map {
public:
	static const std::string mapNames[];
	static const int mapCount;
	static std::string currentMapName;

	static int n;
	static std::vector<Point> points;
	static std::vector<Point> lines;
	static int landLeft, landRight;
	static int landGround;

	static Vector initialPos;
	static Vector initialVel;

	static int initialFuel;
	static int initialAngle;
	static int initialThrust;

	static constexpr int LEFT_BORDER = 0;
	static constexpr int RIGHT_BORDER = 6999;
	static constexpr int TOP_BORDER = 3000;
	static constexpr int BOT_BORDER = 0;

	static void loadMap();
	static void showMap();

	static void init(Agent& agent);
	static bool isCrashed(const State& state);
	static State getState(const Agent& agent);
	static std::string toString(const State& state);

	static inline bool isCrashed(const Agent& agent) {
		return isCrashed(getState(agent));
	}

	static float evaluate(const Agent& agent);
};

#endif /* MAP_HPP */