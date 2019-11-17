#include "Airspace.h"
#include <iostream>

using namespace std;

Airspace& Airspace::getInstance() {
	static Airspace _instance;
	return _instance;
}

Airspace::Airspace() {
	cout << "Airspace instantiated" << endl;
}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}

void Airspace::CollisionPrediction(int period) {
	cout << "Prediction for the next " << period << " seconds." << endl;
}
