#pragma once

#include "Position.h"
#include "Velocity.h"

class Aircraft {
public:
	Aircraft() = default;
	int a_id = -1;
	Position cur_pos;
	Velocity cur_vel;
	Position grid_pos;		// the position within the grid for display purposes

	Aircraft(int, Position, Velocity);
	virtual ~Aircraft();

	void PrintMembers() const;
};
