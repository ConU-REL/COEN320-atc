#pragma once

#include "Aircraft.h"
#include <vector>

using namespace std;

class Display {

public:
	static Display& getInstance();
	void print();
	void link_aircraft(const vector<Aircraft> &ac);
	void print_grid();

private:
	// member variables
	// environment properties
	int x_size = 100;	// airspace x-dim in miles
	int y_size = 100;	// airspace y-dim in miles
	int z_size = 25000;	// airspace z-dim in feet

	int min_x_sep = 3;			// minimum aircraft x separation in miles
	int min_y_sep = min_x_sep;	// minimum aircraft y separation in miles
	int min_z_sep = 1000;		// minimum aircraft z separation in feet
	// end environment properties
	int quadrant_size = 4;		// number of rows and columns per quadrant
	const vector<Aircraft> *aircraft;	// pointer to vector of known aircraft

	// member functions
	char check_position(const int x, const int y) const;

	// constructors
	Display();
	virtual ~Display();
};
