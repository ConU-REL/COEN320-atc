#pragma once

#include "Position.h"
#include "Velocity.h"

class Aircraft {
public:
	Aircraft() = default;
	int s_id; // Id in the system, for the omniscient Airspace to identify unknowns
	int a_id = -1; // Aircraft id
	Position cur_pos;
	Velocity cur_vel;
	Position grid_pos;		// the position within the grid for display purposes
	int a_et; // Aircraft entry time

	Aircraft(int, Position, Velocity, int);
	virtual ~Aircraft();

	void PrintMembers() const;
	void fly(int time = 1); // Move aircraft according to velocity
};
