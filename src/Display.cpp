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
			cout << " ";

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
	cout << aircraft->at(0).grid_pos.px << endl;
}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

