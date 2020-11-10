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
	"cave_correct_side",
	"cave_wrong_side",
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

int Map::landingPointIdx = -1;
std::vector<float> Map::landLengths;
float Map::totalLandLength = 0.f;
int Map::lastCollisionIdx;

void Map::load() {
	std::istream* ins;

	#ifdef LOCAL
	int mapNumber = Options::mapNumber;
	assert(1 <= mapNumber && mapNumber <= mapCount);

	currentMapName = mapNames[mapNumber - 1];
	std::string path = "tests/" + currentMapName + ".txt";
	std::ifstream ifs(path);
	assert(ifs.is_open());
	ins = &ifs;

	#else
		ins = &std::cin;
	#endif

    int lastX = -1, lastY = -1;
    *ins >> pointCount;
    points.reserve(pointCount);

    for (int i = 0; i < pointCount; i++) {
        int x, y;
        *ins >> x >> y;
        points.emplace_back(x, y);

        if (y == lastY) {
            landLeft = lastX;
            landRight = x;
            landGround = y;
        }

        lastX = x;
        lastY = y;
    }

	landLengths.reserve(pointCount - 1);
	for (int i = 0; i < pointCount - 1; ++i) {
		float dx = points[i + 1].x - points[i].x;
		float dy = points[i + 1].y - points[i].y;
		float d = std::sqrt(dx * dx + dy * dy);
		totalLandLength += d;
		landLengths.emplace_back(d);
	}

	*ins >> initialPos.x >> initialPos.y;
	*ins >> initialVel.x >> initialVel.y;
	*ins >> initialFuel >> initialAngle >> initialThrust;

	landMiddle = (landLeft + landRight) / 2;

	for (int i = 0; i < pointCount; ++i)
		if (points[i].x == landLeft && points[i].y == landGround) {
			landingPointIdx = i;
			break;
		}
	assert(landingPointIdx != -1);

	#ifdef DEBUG
	if (Options::verbose)
		show();
	#endif
}

#ifdef DEBUG
void Map::show() {
	std::cout << "\nMap configuration:\n";
	std::cout << "\tMap name: " << currentMapName << "\n";
	std::cout << "\n\tNumber of points: " << pointCount << "\n";
	for (const auto& p : points)
		std::cout << "\t" << p << "\n";
	std::cout << "\n";

	std::cout << "\tLand relative length:\n\t";
	for (const auto& length : landLengths)
		std::cout << length << " ";
	std::cout << "\n\n";

	std::cout << "\tLand relative cummulative distance (left point):\n\t";
	for (int i = 0; i < pointCount - 1; ++i)
		std::cout << walkDistance(i, points[i].x) << " ";
	std::cout << "\n";

	std::cout << "\tLand relative cummulative distance (right point):\n\t";
	for (int i = 0; i < pointCount - 1; ++i)
		std::cout << walkDistance(i, points[i + 1].x) << " ";
	std::cout << "\n\n";

	std::cout << "\tLanding zone: " 
		<< Point(landLeft, landGround) << " " 
		<< Point(landRight, landGround) << "\n\n";
	std::cout << "\tInitial position: " << initialPos << "\n";
	std::cout << "\tInitial velocity: " << initialVel << "\n";
	std::cout << "\tInitial fuel: " << initialFuel << "\n";
	std::cout << "\tInitial angle: " << initialAngle << "\n";
	std::cout << "\tInitial thrust: " << initialThrust << "\n";
}
#endif

void Map::init(Agent& agent) {
	agent.pos = initialPos;
	agent.lastPos = initialPos;
	agent.vel = initialVel;
	agent.angle = initialAngle;
	agent.thrust = initialThrust;
	agent.fuel = initialFuel;
}

bool Map::isFlying(const Agent& agent) {
	return getShipState(agent) == ShipState::Flying;
}

ShipState Map::getShipState(const Agent& agent) {
	const auto& p1 = agent.pos;
	const auto& p2 = agent.lastPos;
	lastCollisionIdx = NO_COLLISION;

	for (int i = 0; i < landingPointIdx; ++i)
		if (collide(p1, p2, points[i], points[i + 1]))
			return lastCollisionIdx = i, ShipState::CrashedOutside;
	for (int i = landingPointIdx + 1; i < pointCount - 1; ++i)
		if (collide(p1, p2, points[i], points[i + 1]))
			return lastCollisionIdx = i, ShipState::CrashedOutside;

	if (collide(p1, p2, points[landingPointIdx], points[landingPointIdx + 1])) {
		if (agent.angle == 0 && std::fabs(agent.vel.x) <= HSPEED_LIMIT && std::fabs(agent.vel.y) <= VSPEED_LIMIT)
			return ShipState::Landed;
		return ShipState::CrashedInside;
	}

	if (agent.pos.x <= LEFT_BORDER)
		return lastCollisionIdx = LEFT_COLLISION, ShipState::CrashedOutside;
	if (agent.pos.x >= RIGHT_BORDER)
		return lastCollisionIdx = RIGHT_COLLISION, ShipState::CrashedOutside;
	if (agent.pos.y <= BOT_BORDER || agent.pos.y >= TOP_BORDER)
		return lastCollisionIdx = TOPBOT_COLLISION, ShipState::CrashedOutside;
	
	if (agent.fuel <= 0)
		return ShipState::FuelLack;

	return ShipState::Flying;
}

float Map::evaluate(const Agent& agent) {
	ShipState shipState = getShipState(agent);
	switch (shipState) {
		case ShipState::Landed:
			return evaluateLanded(agent);
		case ShipState::CrashedInside:
			return evaluateCrashedInside(agent);
		case ShipState::CrashedOutside:
			return evaluateCrashedOutside(agent);
		case ShipState::Flying:
			return evaluateFlying(agent);
		case ShipState::FuelLack:
			return evaluateFuelLack(agent);
	}
	assert(false);
}

float Map::evaluateFlying(const Agent& agent) {
	const auto& p1 = agent.pos;
	const auto p2 = Vector(agent.pos.x, BOT_BORDER);

	int collisionIdx = -1;
	float maxY = -1;

	for (int i = 0; i < pointCount - 1; ++i) {
		const auto& q1 = points[i];
		const auto& q2 = points[i + 1];

		if (collide(p1, p2, q1, q2)) {
			assert(q1.x != q2.x);
			float nom = q2.y * (p1.x - q1.x) + q1.y * (q2.x - p1.x);
			float den = (q2.x - q1.x);
			float y = nom / den;
			assert(y >= std::min(q1.y, q2.y)*0.99f && y <= std::max(q1.y, q2.y)*1.01f);

			if (y <= p1.y && y > maxY) {
				maxY = y;
				collisionIdx = i;
			}
		}
	}

	assert(collisionIdx != -1);
	float distance = walkDistance(collisionIdx, p1.x);
	return 10 * distance +  5 * (initialFuel - agent.fuel);
}

float Map::walkDistance(int landIdx, float xMark) {
	assert(landIdx < pointCount - 1);

	float distance = 0.f;
	const auto& p1 = points[landIdx];
	const auto& p2 = points[landIdx + 1];

	assert(std::min(p1.x, p2.x) <= xMark && xMark <= std::max(p1.x, p2.x));
	assert(p1.x != p2.x);
	float scale = (xMark - p1.x) / (p2.x - p1.x);

	if (landIdx < landingPointIdx) {
		distance += (1.f - scale) * landLengths[landIdx] + 0.5f * landLengths[landingPointIdx]; 
		for (int i = landIdx + 1; i < landingPointIdx; ++i)
			distance += landLengths[i];
	}
	else if (landIdx > landingPointIdx) {
		distance += scale * landLengths[landIdx] + 0.5f * landLengths[landingPointIdx];
		for (int i = landingPointIdx + 1; i < landIdx; ++i)
			distance += landLengths[i];
	}
	else
		distance += (std::max(scale, 1.f - scale) - 0.5f) * landLengths[landIdx];

	return distance;
}

float Map::evaluateCrashedOutside(const Agent& agent) {
	assert(lastCollisionIdx != NO_COLLISION);

	float xMark = -1.f;
	switch (lastCollisionIdx) {
		case LEFT_COLLISION:
			xMark = points[0].x;
			lastCollisionIdx = 0;
			break;
		case RIGHT_COLLISION:
			xMark = points.back().x;
			lastCollisionIdx = pointCount - 2;
			break;
		case TOPBOT_COLLISION:
			return evaluateFlying(agent);
		default:
			assert(collide(agent.pos, agent.lastPos, points[lastCollisionIdx], points[lastCollisionIdx + 1]));
			xMark = (agent.pos.x + agent.lastPos.x) / 2;
	}

	assert(xMark != 1.f);
	assert(lastCollisionIdx < pointCount - 1);

	const auto& p1 = points[lastCollisionIdx];
	const auto& p2 = points[lastCollisionIdx + 1];

	assert(std::min(p1.x, p2.x) - 100.f <= xMark && xMark <= std::max(p1.x, p2.x) + 100.f);
	xMark = clamp<float>(xMark, std::min(p1.x, p2.x), std::max(p1.x, p2.x));

	float distance = walkDistance(lastCollisionIdx, xMark) * 10;
	float hSpeedAboveLimit = std::pow(std::max(0.f, std::fabs(agent.vel.x) - HSPEED_LIMIT), 2.5f);
	float vSpeedAboveLimit = std::pow(std::max(0.f, std::fabs(agent.vel.y) - VSPEED_LIMIT / 2), 3.f);
	return distance + hSpeedAboveLimit + vSpeedAboveLimit + 5 * (initialFuel - agent.fuel);
}

float Map::evaluateCrashedInside(const Agent& agent) {
	static constexpr int ABOVE_LIMIT_PENALTY = 1000;
	float eval = 0.f;

	if (std::fabs(agent.vel.x) > HSPEED_LIMIT)
		eval += ABOVE_LIMIT_PENALTY + std::pow(std::fabs(agent.vel.x) - HSPEED_LIMIT / 1.5f, 2.5f);
	if (std::fabs(agent.vel.y) > VSPEED_LIMIT)
		eval += 2 * ABOVE_LIMIT_PENALTY + std::pow(std::fabs(agent.vel.y) - VSPEED_LIMIT / 2, 3.f);
	if (agent.angle != 0)
		eval += ABOVE_LIMIT_PENALTY + std::pow(std::abs(agent.angle), 2.f);;

	return eval + 5 * (initialFuel - agent.fuel);
}

float Map::evaluateLanded(const Agent& agent) {
	// assert(false);
	assert(agent.fuel > 0);
	assert(agent.fuel <= initialFuel);
	return 5 * (initialFuel - agent.fuel);
}

float Map::evaluateFuelLack(const Agent& agent) {
	assert(false);
	return initialFuel + 10000;
}

#ifdef DEBUG
std::ostream& operator<<(std::ostream& out, const ShipState& shipState) {
	switch (shipState) {
		case ShipState::Landed:
			return out << "Landed";
		case ShipState::CrashedInside:
			return out << "CrashedInside";
		case ShipState::CrashedOutside:
			return out << "CrashedOutside";
		case ShipState::FuelLack:
			return out << "FuelLack";
		case ShipState::Flying:
			return out << "Flying";
	}
	assert(false);
}
#endif
