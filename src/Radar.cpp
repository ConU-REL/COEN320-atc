#include "Radar.h"
#include <iostream> // FOR DEBUGGING

using namespace std;

Radar& Radar::getInstance() {
	static Radar _instance;
	return _instance;
}

Radar::Radar() {
	cout << "Radar instantiated" << endl;

}

Radar::~Radar() {
	// TODO Auto-generated destructor stub
}

