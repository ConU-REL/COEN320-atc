#pragma once

#include "Position.h"
#include "Velocity.h"

#ifndef AIRCRAFT_H_
#define AIRCRAFT_H_

class Aircraft {
public:
	int a_id;
	Position cur_pos;
	Velocity cur_vel;

	Aircraft();
	virtual ~Aircraft();
};

#endif /* AIRCRAFT_H_ */
