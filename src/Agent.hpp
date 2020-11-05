#ifndef AGENT_HPP
#define AGENT_HPP

#include "Common.hpp"

struct Action {
	int dAngle;
	int dThrust;
};

class Agent {
public:
	void apply(const Action& action);

private:
	Point pos;
	Point vel;
	int angle;
	int thrust;
};

#endif /* AGENT_HPP */