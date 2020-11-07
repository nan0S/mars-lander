#ifndef AGENT_HPP
#define AGENT_HPP

#include "Common.hpp"

#include <iostream>

struct Action {
	int dAngle;
	int dThrust;

	static constexpr int MIN_DANGLE = -15;
	static constexpr int MAX_DANGLE = 15;
	static constexpr int MIN_DTHRUST = -1;
	static constexpr int MAX_DTHRUST = 1;

	static Action getRandom();
	friend std::ostream& operator<<(std::ostream& out, const Action& action);
};

class Agent {
public:
	void apply(const Action& action);

	Vector pos;
	Vector lastPos;
	Vector vel;
	int angle;
	int thrust;
	int fuel;

	friend std::ostream& operator<<(std::ostream& out, const Agent& agent);

private:
	constexpr static int MIN_ANGLE = -90;
	constexpr static int MAX_ANGLE = +90;
	constexpr static int MIN_THRUST = 0;
	constexpr static int MAX_THRUST = 4;

	constexpr static float G = 3.711;
};

#endif /* AGENT_HPP */