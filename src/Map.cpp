#include "Map.hpp"
#include "Options.hpp"

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

int Map::pointCount;
std::vector<Point> Map::points;

int Map::landLeft;
int Map::landRight;
int Map::landMiddle;
int Map::landGround;
Vector Map::initialPos;
Vector Map::initialVel;
int Map::initialFuel;
int Map::initialAngle;
int Map::initialThrust;

void Map::load() {
	int mapNumber = Options::mapNumber;
	assert(1 <= mapNumber && mapNumber <= mapCount);

	currentMapName = mapNames[mapNumber - 1];
	std::string path = "tests/" + currentMapName + ".txt";
	std::ifstream ifs(path);
	assert(ifs.is_open());

	ifs >> pointCount;
	points.reserve(pointCount);
	for (int i = 0; i < pointCount; ++i) {
		int x, y;
		ifs >> x >> y;
		points.emplace_back(x, y);
	}

	ifs >> landLeft >> landRight;
	ifs >> landGround;
	ifs >> initialPos.x >> initialPos.y;
	ifs >> initialVel.x >> initialVel.y;
	ifs >> initialFuel >> initialAngle >> initialThrust;

	landMiddle = (landLeft + landRight) / 2;

	if (Options::verbose)
		show();
}

void Map::show() {
	std::cout << "\nMap configuration:\n";
	std::cout << "\tMap name: " << currentMapName << "\n";
	std::cout << "\n\tNumber of points: " << pointCount << "\n";
	for (const auto& p : points)
		std::cout << "\t" << p << "\n";
	std::cout << "\n";

	std::cout << "\tLanding zone: " 
		<< Point(landLeft, landGround) << " " 
		<< Point(landRight, landGround) << "\n\n";
	std::cout << "\tInitial position: " << initialPos << "\n";
	std::cout << "\tInitial velocity: " << initialVel << "\n";
	std::cout << "\tInitial fuel: " << initialFuel << "\n";
	std::cout << "\tInitial angle: " << initialAngle << "\n";
	std::cout << "\tInitial thrust: " << initialThrust << "\n";
}

void Map::init(Agent& agent) {
	agent.pos = initialPos;
	agent.lastPos = initialPos;
	agent.vel = initialVel;
	agent.angle = initialAngle;
	agent.thrust = initialThrust;
	agent.fuel = initialFuel;
}

bool Map::isCrashed(const Agent& agent) {
	if (agent.fuel <= 0)
		return true;

	if (agent.pos.x <= LEFT_BORDER || agent.pos.x >= RIGHT_BORDER)
		return true;
	if (agent.pos.y <= BOT_BORDER || agent.pos.y >= TOP_BORDER)
		return true;

	const auto& p1 = agent.pos;
	const auto& p2 = agent.lastPos;

	for (int i = 0; i < pointCount - 1; ++i) {
		const auto& q1 = points[i];
		const auto& q2 = points[i + 1];

		if (collide(p1, p2, q1, q2))
			return true;
	} 

	return false;
}

float Map::evaluate(const Agent& agent) {
	float dx = agent.pos.x - landMiddle;
	float dy = agent.pos.y - landGround;
	float d = std::sqrt(dx * dx + dy * dy);
	// return d;
	assert(d <= 30000.f);
	return 30000.f - d;
}