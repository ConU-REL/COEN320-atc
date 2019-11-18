#pragma once
#include "Velocity.h"

struct Position {
	int px;
	int py;
	int pz;

	void operator+(Velocity v) {
		px = px + v.vx;
		py = py + v.vy;
		pz = pz + v.vz;
	}

	void operator+=(Velocity v) {
		px += v.vx;
		py += v.vy;
		pz += v.vz;
	}

	void operator-(Velocity v) {
		px = px - v.vx;
		py = py - v.vy;
		pz = pz - v.vz;
	}

	void operator-=(Velocity v) {
		px -= v.vx;
		py -= v.vy;
		pz -= v.vz;
	}
};
