#pragma once

#include "Velocity.h"

struct Message {
	int dest_id;
	// 1 = Report position and velocity
	// 2 = Enter holding pattern
	// 3 = Exit holding pattern
	// 4 = Change velocity
	// 5 = Change elevation
	// 6 = BROADCAST - Report position and velocity
	// 7 = BROADCAST - Enter holding pattern
	// 8 = BROADCAST - Exit holding pattern
	int type;
	Velocity vel;
	int elevation;
};
