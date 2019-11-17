#include <iostream>
#include "Airspace.h"
#include "Communications.h"
#include "Radar.h"

using namespace std;

void myFunc() {
	cout << "hi" << endl;
}

int main() {
	Airspace& airspace = Airspace::getInstance();
	Communications& comms = Communications::getInstance();
	Radar& radar = Radar::getInstance();


	airspace.CollisionPrediction();
	airspace.CollisionPrediction(20);

	return 0;
}
