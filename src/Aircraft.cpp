#include "Aircraft.h"

using namespace std;

Aircraft::Aircraft(int sid, int id, Position position, Velocity velocity, int entry_time) {
	s_id = sid;
	a_id = id;
	cur_pos = position;
	cur_vel = velocity;
	storage_vel = velocity;
	a_et = entry_time;

	grid_pos.px = ceil((float)(cur_pos.px/5280)/3);
	grid_pos.py = ceil((float)(cur_pos.py/5280)/3);
	grid_pos.pz = cur_pos.pz;
}

Aircraft::~Aircraft() {
	// TODO Auto-generated destructor stub
}

void Aircraft::fly(int time) {
	for (int i = 0; i < time; i++) {
		if (holding) {
			if (holdingTimer-- <= 0) {
				// make a left turn when timer is up to make a circuit
				if (cur_vel.vx > 0) {
					cur_vel.vx = 0;
					cur_vel.vy = HOLDING_SPEED;
				}
				else if (cur_vel.vx < 0) {
					cur_vel.vx = 0;
					cur_vel.vy = -HOLDING_SPEED;
				}
				else if (cur_vel.vy > 0) {
					cur_vel.vx = -HOLDING_SPEED;
					cur_vel.vy = 0;
				}
				else if (cur_vel.vy < 0) {
					cur_vel.vx = HOLDING_SPEED;
					cur_vel.vy = 0;
				}
				holdingTimer = HOLDING_TIMER;
			}
		}
		else {
			cur_pos += cur_vel;
			cur_pos.pz = cur_pos.pz - cur_vel.vz; // Ignore z axis in our airspace under normal conditions

		}

		if (ascendDescend) {
			if (cur_pos.pz < targetElevation) { // We need to ascend
				if (cur_pos.pz + ASCENT_VELOCITY > targetElevation) { // Don't overshoot
					cur_pos.pz = targetElevation;
					cur_vel.vz = storage_vel.vz;
					ascendDescend = false;
				}
				else {
					cur_vel.vz = ASCENT_VELOCITY;
					cur_pos.pz = cur_pos.pz + ASCENT_VELOCITY;
				}
			}
			else { // We need to descend // Also triggers if we happened to exactly hit our target
				if (cur_pos.pz - ASCENT_VELOCITY < targetElevation) { // Don't overshoot
					cur_pos.pz = targetElevation;
					cur_vel.vz = storage_vel.vz;
					ascendDescend = false;
				}
				else {
					cur_vel.vz = -ASCENT_VELOCITY;
					cur_pos.pz = cur_pos.pz - ASCENT_VELOCITY;
				}
			}
		}

		grid_pos.px = ceil((float)(cur_pos.px/5280)/3);
		grid_pos.py = ceil((float)(cur_pos.py/5280)/3);
		grid_pos.pz = cur_pos.pz;
	}
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

void Aircraft::PrintFullMembers() const {
	cout << "SID: " << s_id;
	if (a_id == -1) {
		cout << " AID: UNKNOWN ";
	}
	else {
		cout << " AID:" << a_id << " ";
	}
	cout << "POSITION: " << cur_pos.px << "," << cur_pos.py << "," << cur_pos.pz << " "
			"VELOCITY: " << cur_vel.vx << "," << cur_vel.vy << "," << cur_vel.vz << " "
			"ENTRY TIME: " << a_et << endl;
}

void Aircraft::StartHolding() {
	holdingTimer = HOLDING_TIMER;
	cur_vel = holding_velocity;
	holding = true;
}

void Aircraft::StopHolding() {
	cur_vel = storage_vel;
	holding = false;
}

string Aircraft::Response() {
	string response = "REPORT FROM ID: " + to_string(a_id) + " " +
						"POSITION: " + to_string(cur_pos.px) + "," + to_string(cur_pos.py) + "," + to_string(cur_pos.pz) + " "
						"VELOCITY: " + to_string(cur_vel.vx) + "," + to_string(cur_vel.vy) + "," + to_string(cur_vel.vz) + "\n";
	return response;
}

void Aircraft::ChangeVelocity(Velocity newVel) {
	cur_vel = newVel;
	storage_vel = newVel;
}

void Aircraft::ChangeElevation(int elevation) {
	targetElevation = elevation;
	ascendDescend = true;
	if (cur_pos.pz == targetElevation) {
		cur_vel.vz = 0;
	}
	else if (cur_pos.pz < targetElevation) {
		cur_vel.vz = ASCENT_VELOCITY;
	}
	else {
		cur_vel.vz = -ASCENT_VELOCITY;
	}

}
