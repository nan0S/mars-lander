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

const std::string testPath = "tests/";

int Map::n;
std::vector<Point> Map::points;
std::vector<Point> Map::lines;
int Map::landLeft;
int Map::landRight;
int Map::landGround;

Vector Map::initialPos;
Vector Map::initialVel;

int Map::initialFuel;
int Map::initialAngle;
int Map::initialThrust;

void Map::loadMap() {
	int mapNumber = Options::mapNumber;
	assert(1 <= mapNumber && mapNumber <= mapCount);

	currentMapName = mapNames[mapNumber - 1];
	std::string path = testPath + currentMapName + ".txt";

	std::ifstream ifs(path);
	assert(ifs.is_open());

	ifs >> n;
	points.reserve(n);
	lines.reserve(n + 1);
	Point lastPoint { LEFT_BORDER, TOP_BORDER };


	for (int i = 0; i < n; ++i) {
		int x, y;
		ifs >> x >> y;
		points.emplace_back(x, y);
		lines.emplace_back(x - lastPoint.x, y - lastPoint.y);
		lastPoint = {x, y};
	}
	lines.emplace_back(RIGHT_BORDER - lastPoint.x, TOP_BORDER - lastPoint.y);
	
	ifs >> landLeft >> landRight;
	ifs >> landGround;
	ifs >> initialPos.x >> initialPos.y;
	ifs >> initialVel.x >> initialVel.y;
	ifs >> initialFuel >> initialAngle >> initialThrust;
}

void Map::showMap() {
	std::cout << "\nMap name: " << currentMapName << "\n";
	std::cout << "\nNumber of points: " << n << "\n";
	for (const auto& p : points)
		std::cout << p << "\n";
	std::cout << "\n";

	std::cout << "Landing zone: " << Point(landLeft, landGround) << " " << Point(landRight, landGround) << "\n\n";
	std::cout << "Initial position: " << initialPos << "\n";
	std::cout << "Initial velocity: " << initialVel << "\n";
	std::cout << "Initial fuel: " << initialFuel << "\n";
	std::cout << "Initial angle: " << initialAngle << "\n";
	std::cout << "Initial thrust: " << initialThrust << "\n\n";
}

void Map::init(Agent& agent) {
	agent.pos = initialPos;
	agent.lastPos = initialPos;
	agent.vel = initialVel;
	agent.angle = initialAngle;
	agent.thrust = initialThrust;
	agent.fuel = initialFuel;
}

bool Map::isCrashed(const State& state) {
	switch (state) {
		case State::Fuel:
		case State::Crash:
			return true;
		default:
			return false;
	}
}

State Map::getState(const Agent& agent) {
	if (agent.fuel <= 0)
		return State::Fuel;

	if (agent.pos.x <= LEFT_BORDER || agent.pos.x >= RIGHT_BORDER)
		return State::Crash;
	if (agent.pos.y <= BOT_BORDER || agent.pos.y >= TOP_BORDER)
		return State::Crash;

	auto& p1 = agent.pos;
	auto& p2 = agent.lastPos;

	for (int i = 0; i < n - 1; ++i) {
		auto& q1 = points[i];
		auto& q2 = points[i + 1];

		if (collide(p1, p2, q1, q2))
			return State::Crash;
	}

	return State::Good;
}

std::ostream& operator<<(std::ostream& out, const State& state) {
	switch (state) {
		case State::Crash:
			return out << "Crash";
		case State::Fuel:
			return out << "Fuel";
		case State::Good:
			return out << "Good";
	}
	assert(false);
}

float Map::evaluate(const Agent& agent) {
	float dx = agent.pos.x - (landLeft + landRight) / 2;
	float dy = agent.pos.y - landGround;
	float d = std::sqrt(dx * dx + dy * dy);
	assert(d <= 30000);
	return 30000 - d;
}