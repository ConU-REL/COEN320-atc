#pragma once

#include "Position.h"
#include "Velocity.h"

class Aircraft {
public:
	int a_id;
	Position cur_pos;
	Velocity cur_vel;

	Aircraft();
	virtual ~Aircraft();
};
