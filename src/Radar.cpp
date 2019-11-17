#include "Radar.h"
#include <iostream> // FOR DEBUGGING

using namespace std;

Radar& Radar::getInstance() {
	static Radar _instance;
	return _instance;
}

Radar::Radar(int scan_interval)
 : m_Scan_Interval(scan_interval)
{
	cout << "Radar instantiated" << endl;


}

Radar::~Radar() {
	// TODO Auto-generated destructor stub
}


void Radar::CollisionPrediction(int period) {
	cout << "Prediction for the next " << period << " seconds." << endl;

	// If you detect a collision within the period send a message to the user via COMMUNICATIONS
}

void Radar::ProcessTime() {
	// This is just a stub for now

	// OBTAIN mutex for accessing m_Aircrafts
	//m_Aircrafts = m_Airspace.Scan();
	// RELEASE mutex for accesing m_Aircrafts
}

bool Radar::AdvanceTime() {
	if (true) { // TEMP FOR TESTING, normally (m_Time % m_Scan_Interval == 0) {
		// OBTAIN mutex for accessing m_Aircrafts
		m_Aircrafts = m_Airspace.Scan();
		// Release mutex for accessing m_Aircrafts
	}
	return true;
}

vector<Aircraft> Radar::Report() {
	// OBTAIN mutex for accessing m_Aircrafts
	return m_Aircrafts;
	// Release mutex for accessing m_Aircrafts
}
