#include "Aircraft.h"

Aircraft::Aircraft(int id, Position position, Velocity velocity) {
	a_id = id;
	cur_pos = position;
	cur_vel = velocity;
}

Aircraft::~Aircraft() {
	// TODO Auto-generated destructor stub
}

