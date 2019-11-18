#include "Aircraft.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

Aircraft::Aircraft(int id, Position position, Velocity velocity) {
	a_id = id;
	cur_pos = position;
	cur_vel = velocity;

	grid_pos.px = ceil((float)(cur_pos.px/5280)/3);
	grid_pos.py = ceil((float)(cur_pos.py/5280)/3);
	grid_pos.pz = cur_pos.pz;
}

Aircraft::~Aircraft() {
	// TODO Auto-generated destructor stub
}

void Aircraft::PrintMembers() const {
	cout << "ID: ";
	if (a_id == -1) {
		cout << "UNKNOWN ";
	}
	else {
		cout << a_id << " ";
	}
	cout << "POSITION: " << cur_pos.px << "," << cur_pos.py << "," << cur_pos.pz << " "
			"VELOCITY: " << cur_vel.vx << "," << cur_vel.vy << "," << cur_vel.vz << " " << endl;
}
