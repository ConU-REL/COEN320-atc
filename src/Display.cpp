#include "Display.h"
#include <iostream>
#include <math.h>

using namespace std;


Display& Display::getInstance() {
	static Display _instance;
	return _instance;
}


Display::Display() {
	cout << "Display instantiated" << endl;
	this->aircraft = NULL;		// initialize to NULL to suppress IDE warning
}


void Display::print_grid(){
	int grid_size_x = ceil((float)(x_size / min_x_sep) / quadrant_size) * quadrant_size;
	int grid_size_y = ceil((float)(y_size / min_y_sep) / quadrant_size) * quadrant_size;
	int elevation_chars = ceil((float)z_size/min_z_sep);
	int columns = grid_size_x/quadrant_size;

	// DEBUG
//	cout << "Grid Dimensions: " << grid_size_x << " x " << grid_size_y << endl;
//	cout << elevation_chars << " characters required to show Elevation Separation\n";

	int row = 1;
	char col = 65;
	for(int i=0; i < grid_size_x; i++){
		if(!(i % quadrant_size)){
			cout << " ";
		}
		if((float)(i % quadrant_size) == 2){
			cout << col++;
		} else {
			cout << " ";
		}

	}
	cout << "\n";
	for(int i=0; i < grid_size_x; i++){
		if(!(i % quadrant_size)){
			cout << string(grid_size_x+columns+1, '-') << endl;
		}
		for(int j=0; j < grid_size_y; j++){
			if(!(j % quadrant_size)){
				cout << "|";
			}

			// TODO: PRINT AIRPLANES HERE
			cout << check_position(j, i);

			/*for (Aircraft ac : *aircraft) {
				ac.PrintMembers();	// The position of the aircraft are definitely updated in this scope
			}*/

		}
		cout << "|";
		if((float)(i % quadrant_size) == 1){
			cout << row++;
		}
		cout << "\n";
	}
	cout << string(grid_size_x+columns+1, '-') << endl;
}


void Display::link_aircraft(const vector<Aircraft> &ac){
	aircraft = &ac;
}

char Display::check_position(const int x, const int y) const{
	for(auto ac : *aircraft){
		if(ac.grid_pos.px == x && ac.grid_pos.py == y){
			//cout << "x: " << x << " y: " << y << endl; // These numbers never change for a given aircraft once it first appears.
			//ac.PrintMembers(); // The position of the aircraft are definitely updated in this scope
			return 89 - ceil((float)ac.grid_pos.pz/min_z_sep);
		}
	}
	return 0;
}


Display::~Display() {
	// TODO Auto-generated destructor stub
}

