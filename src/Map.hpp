#ifndef MAP_HPP
#define MAP_HPP

#include "Common.hpp"
#include "Agent.hpp"

#include <string>
#include <vector>

enum class ShipState {
	Landed,
	CrashedInside,
	CrashedOutside,
	FuelLack,
	Flying
};

#ifndef NDEBUG
std::ostream& operator<<(std::ostream& out, const ShipState& shipState);
#endif

class Map {
public:
	static void load();
	static void init(Agent& agent);
	static bool isFlying(const Agent& agent);
	static ShipState getShipState(const Agent& agent);
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

	static int landingPointIdx;
	static std::vector<float> landLengths;
	static int lastCollisionIdx;

	static constexpr int VSPEED_LIMIT = 40;
	static constexpr int HSPEED_LIMIT = 20;
	static constexpr float VSPEED_ABOVE_LIMIT_SCALE = 4.f;
	static constexpr float HSPEED_ABOVE_LIMIT_SCALE = 2.f;
	static constexpr float MAX_HSPEED = HSPEED_LIMIT * HSPEED_ABOVE_LIMIT_SCALE;
	static constexpr float MAX_VSPEED = VSPEED_LIMIT * VSPEED_ABOVE_LIMIT_SCALE;
	static constexpr int MAX_ANGLE = 90;

	static float evaluateFlying(const Agent& agent);
	static float evaluateCrashedOutside(const Agent& agent);
	static float evaluateCrashedInside(const Agent& agent);
	static float evaluateLanded(const Agent& agent);
	static float evaluateFuelLack(const Agent& agent);

	static float walkDistance(int landIdx, float xMark);
};

#endif /* MAP_HPP */