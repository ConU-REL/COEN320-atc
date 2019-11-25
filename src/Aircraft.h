#pragma once

#include "Position.h"
#include "Velocity.h"
#include <iostream>
#include <math.h>
#include <string>

#define HOLDING_TIMER 10 // duplicate in airspace.h, avoiding cyclic dependency
#define HOLDING_SPEED 50

class Aircraft {
public:
	Aircraft() = default;
	int s_id; // Id in the system, for the omniscient Airspace to identify unknowns
	int a_id = -1; // Aircraft id
	Position cur_pos;
	Velocity cur_vel;
	Position grid_pos;		// the position within the grid for display purposes
	int a_et; // Aircraft entry time

	bool holding = false;
	int holdingTimer = HOLDING_TIMER;
	Velocity holding_velocity{HOLDING_SPEED,0,0};
	Velocity storage_vel;
	void StartHolding();
	void StopHolding();
	std::string Response();

	Aircraft(int, int, Position, Velocity, int);
	virtual ~Aircraft();

	void PrintMembers() const;
	void PrintFullMembers() const;
	void fly(int time = 1); // Move aircraft according to velocity

	void ChangeElevation(int);
	void ChangeVelocity(Velocity);
};
