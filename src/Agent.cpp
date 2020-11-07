#include "Agent.hpp"

#include <cmath>

Action Action::getRandom() {
	Action action;
	action.dAngle = Random::rand(MIN_DANGLE, MAX_DANGLE);
	action.dThrust = Random::rand(MIN_DTHRUST, MAX_DTHRUST);
	return action;
}

std::ostream& operator<<(std::ostream& out, const Action& action) {
	return out << "(dAngle=" << action.dAngle << " , dThrust=" << action.dThrust << ")";  
}
 
void Agent::apply(const Action& action) {
	angle = clamp(angle + action.dAngle, MIN_ANGLE, MAX_ANGLE);
	thrust = clamp(thrust + action.dThrust, MIN_THRUST, MAX_THRUST);

	vel.x -= sin(angle) * thrust;
	vel.y += cos(angle) * thrust - G;

	lastPos = pos;

	pos.x += vel.x;
	pos.y += vel.y;

	fuel -= thrust;
}

std::ostream& operator<<(std::ostream& out, const Agent& agent) {
	return out << "(x=" << agent.pos.x << " , y=" << agent.pos.y
		<< ", xvel=" << agent.vel.x << " , yvel=" << agent.vel.y
		<< ", angle=" << agent.angle << ", thrust=" << agent.thrust
		<< ", fuel=" << agent.fuel << ")";
}