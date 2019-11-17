#include "Aircraft.h"
#include <iostream>
#include <string>

using namespace std;

Aircraft::Aircraft(int id, Position position, Velocity velocity) {
	a_id = id;
	cur_pos = position;
	cur_vel = velocity;
}

Aircraft::~Aircraft() {
	// TODO Auto-generated destructor stub
}

void Aircraft::PrintMembers() {
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
