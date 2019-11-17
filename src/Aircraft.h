#pragma once

#include "Position.h"
#include "Velocity.h"

class Aircraft {
public:
	Aircraft() = default;
	int a_id = -1;
	Position cur_pos;
	Velocity cur_vel;

	Aircraft(int, Position, Velocity);
	virtual ~Aircraft();
};
