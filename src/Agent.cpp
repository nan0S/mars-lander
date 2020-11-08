#include "Agent.hpp"
#include "Common.hpp"

Action Action::getRandom() {
	Action action;
	action.dAngle = Random::rand(MIN_DANGLE, MAX_DANGLE);
	action.dThrust = Random::rand(MIN_DTHRUST, MAX_DTHRUST);
	return action;
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
