#include "Aircraft.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

Aircraft::Aircraft(int sid, int id, Position position, Velocity velocity, int entry_time) {
	s_id = sid;
	a_id = id;
	cur_pos = position;
	cur_vel = velocity;
	a_et = entry_time;

	grid_pos.px = ceil((float)(cur_pos.px/5280)/3);
	grid_pos.py = ceil((float)(cur_pos.py/5280)/3);
	grid_pos.pz = cur_pos.pz;
}

Aircraft::~Aircraft() {
	// TODO Auto-generated destructor stub
}

void Aircraft::fly(int time) {
	cur_pos += cur_vel;
	cur_pos.pz = cur_pos.pz - cur_vel.vz; // Ignore z axis in our airspace

	grid_pos.px = ceil((float)(cur_pos.px/5280)/3);
	grid_pos.py = ceil((float)(cur_pos.py/5280)/3);
}

void Aircraft::PrintMembers() const {
	if (a_id == -1) {
		cout << "ID: UNKNOWN ";
	}
	else {
		cout << "ID:" << a_id << " ";
	}
	cout << "POSITION: " << cur_pos.px << "," << cur_pos.py << "," << cur_pos.pz << " "
			"VELOCITY: " << cur_vel.vx << "," << cur_vel.vy << "," << cur_vel.vz << " " << endl;
}
