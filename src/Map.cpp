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

int Map::landingPointIdx = -1;
std::vector<float> Map::landLengths;
int Map::lastCollisionIdx;

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

	landLengths.reserve(pointCount - 1);
	float totalLandLength = 0.f;
	for (int i = 0; i < pointCount - 1; ++i) {
		float dx = points[i + 1].x - points[i].x;
		float dy = points[i + 1].y - points[i].y;
		float d = std::sqrt(dx * dx + dy * dy);
		totalLandLength += d;
		landLengths.emplace_back(d);
	}
	for (auto& length : landLengths)
		length /= totalLandLength;

	ifs >> landLeft >> landRight;
	ifs >> landGround;
	ifs >> initialPos.x >> initialPos.y;
	ifs >> initialVel.x >> initialVel.y;
	ifs >> initialFuel >> initialAngle >> initialThrust;

	landMiddle = (landLeft + landRight) / 2;

	for (int i = 0; i < pointCount; ++i)
		if (points[i].x == landLeft) {
			landingPointIdx = i;
			break;
		}
	assert(landingPointIdx != -1);

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

bool Map::isFlying(const Agent& agent) {
	return getShipState(agent) == ShipState::Flying;
}

ShipState Map::getShipState(const Agent& agent) {
	const auto& p1 = agent.pos;
	const auto& p2 = agent.lastPos;
	lastCollisionIdx = -2;

	for (int i = 0; i < landingPointIdx; ++i)
		if (collide(p1, p2, points[i], points[i + 1])) {
			lastCollisionIdx = i;
			return ShipState::CrashedOutside;
		}
	for (int i = landingPointIdx + 1; i < pointCount - 1; ++i)
		if (collide(p1, p2, points[i], points[i + 1])) {
			lastCollisionIdx = -1;
			return ShipState::CrashedOutside;
		}

	if (collide(p1, p2, points[landingPointIdx], points[landingPointIdx + 1])) {
		if (agent.angle == 0 && std::fabs(agent.vel.x) <= HSPEED_LIMIT && std::fabs(agent.vel.y) <= VSPEED_LIMIT)
			return ShipState::Landed;
		return ShipState::CrashedInside;
	}

	if (agent.pos.x <= LEFT_BORDER || agent.pos.x >= RIGHT_BORDER) {
		lastCollisionIdx = -1;
		return ShipState::CrashedOutside;
	}
	if (agent.pos.y <= BOT_BORDER || agent.pos.y >= TOP_BORDER) {
		lastCollisionIdx = -1;
		return ShipState::CrashedOutside;
	}
	
	if (agent.fuel <= 0)
		return ShipState::FuelLack;

	return ShipState::Flying;
}

float Map::evaluate(const Agent& agent) {
	ShipState shipState = getShipState(agent);
	switch (shipState) {
		case ShipState::Landed:
			return 0.9f + 0.1f * evaluateLanded(agent);
		case ShipState::CrashedInside:
			return 0.5f + 0.2f * evaluateCrashedInside(agent);
		case ShipState::Flying:
			return 0.4f + 0.1f * evaluateFlying(agent);
		case ShipState::CrashedOutside:
			return 0.3f + 0.1f * evaluateCrashedOutside(agent);
		case ShipState::FuelLack:
			return 0.0f + 0.3f * evaluateFuelLack(agent);
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
			assert(y >= std::min(q1.y, q2.y) && y <= std::max(q1.y, q2.y));

			if (y <= p1.y && y > maxY) {
				maxY = y;
				collisionIdx = i;
			}
		}
	}

	assert(collisionIdx != -1);
	float distance = walkDistance(collisionIdx, p1.x);
	return 1.f - distance;
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

	assert(0.f <= distance && distance <= 1.f);

	return distance;
}

float Map::evaluateCrashedOutside(const Agent& agent) {
	assert(lastCollisionIdx != -2);
	if (lastCollisionIdx == -1)
		return std::numeric_limits<float>::epsilon();

	assert(lastCollisionIdx < pointCount - 1);

	const auto& p1 = points[lastCollisionIdx];
	const auto& p2 = points[lastCollisionIdx + 1];
	float xMark = agent.pos.x;

	assert(collide(agent.pos, agent.lastPos, p1, p2));

	#ifndef NDEBUG
	if (Options::verbose)
		std::cout << "xMark:\t" << xMark << ", p1.x:\t" << p1.x << ", p2.x:\t" << p2.x << std::endl;
	#endif

	assert(std::min(p1.x, p2.x) - 200.f <= xMark && xMark <= std::max(p1.x, p2.x) + 200.f);
	xMark = clamp<float>(xMark, std::min(p1.x, p2.x), std::max(p1.x, p2.x));

	float distance = walkDistance(lastCollisionIdx, xMark);
	assert(0.f <= distance && distance <= 1.f);
	return 1.f - distance;
}

float Map::evaluateCrashedInside(const Agent& agent) {
	static constexpr float ABOVE_LIMIT_SCALE = 0.05f;
	static_assert(3.f * ABOVE_LIMIT_SCALE <= 1.f, "ABOVE_LIMIT_SCALE has to be less that one third!");
	static constexpr float REST_SCALE = 1.f - ABOVE_LIMIT_SCALE * 3.f;

	float eval = 0.f;
	float hSpeedAboveLimit = 0.f;
	float vSpeedAboveLimit = 0.f;
	float angleAboveLimit = 0.f;

	if (std::fabs(agent.vel.x) > HSPEED_LIMIT) {
		eval += ABOVE_LIMIT_SCALE;
		hSpeedAboveLimit = std::min(std::fabs(agent.vel.x) - HSPEED_LIMIT, MAX_HSPEED) / MAX_HSPEED;
		// eval += ABOVE_LIMIT_SCALE + std::min(std::fabs(agent.vel.x) - HSPEED_LIMIT, MAX_HSPEED) / MAX_HSPEED * REST_SCALE / 3.f;
	}
	if (std::fabs(agent.vel.y) > VSPEED_LIMIT) {
		eval += ABOVE_LIMIT_SCALE;
		vSpeedAboveLimit = std::min(std::fabs(agent.vel.y) - VSPEED_LIMIT, MAX_VSPEED) / MAX_VSPEED;
		// eval += ABOVE_LIMIT_SCALE + std::min(std::fabs(agent.vel.y) - VSPEED_LIMIT, MAX_VSPEED) / MAX_VSPEED * REST_SCALE / 3.f;
	}
	if (agent.angle != 0) {
		eval += ABOVE_LIMIT_SCALE;
		angleAboveLimit = float(std::abs(agent.angle)) / MAX_ANGLE;
		// eval += ABOVE_LIMIT_SCALE + float(std::abs(agent.angle)) / MAX_ANGLE * REST_SCALE / 3.f;
	}
	
	eval += (hSpeedAboveLimit + vSpeedAboveLimit + angleAboveLimit) * REST_SCALE / 3.f;
	assert(0.f <= eval && eval <= 1.f);

	#ifndef NDEBUG
	if (Options::verbose)
		std::cout << "eval:\t" << eval << ", hSpeedAboveLimit:\t" << hSpeedAboveLimit << ", vSpeedAboveLimit:\t" << vSpeedAboveLimit << ", angleAboveLimit:\t" << angleAboveLimit << std::endl;
	#endif

	return 1.f - eval;
}

float Map::evaluateLanded(const Agent& agent) {
	assert(agent.fuel > 0);
	float eval = float(agent.fuel) / initialFuel;
	assert(0.f <= eval && eval <= 1.f);
	return eval;
}

float Map::evaluateFuelLack(const Agent& agent) {
	return 0.f;	
}

#ifndef NDEBUG
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