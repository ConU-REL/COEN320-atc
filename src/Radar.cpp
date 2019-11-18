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
	m_ProcessingThread = new thread{ &Radar::ProcessTime, this };
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

	while (m_SimulationRunning)
		{
			// Wait for a unit of time to pass
			unique_lock<mutex> timelock(m_TimeMutex);
			m_Cond_ScanTime.wait(timelock);
			/*m_Cond_Time.wait(timelock,
				[&current_time = m_Time, &lt = last_time]
				 { return (current_time > lt); });*/
				// To prevent spurious wakeup make sure time has actually passed

			lock_guard<mutex> aircraftlock(m_AircraftMutex);
			m_Aircrafts = m_Airspace.Scan();

			if (m_Time % LOG_INTERVAL == 0) {
				// Call the logger
			}
		}
}

bool Radar::AdvanceTime(int seconds) {
	for (int i = 1; i <= seconds; i++) {
		{
			lock_guard<mutex> timelock(m_TimeMutex);
			m_Time++;
			if (m_Time % RADAR_INTERVAL == 0) {
				m_Cond_ScanTime.notify_all(); // Let ProcessTime run
			}
		}

	}
	return true;
}

vector<Aircraft> Radar::Report() {
	// OBTAIN mutex for accessing m_Aircrafts
	return m_Aircrafts;
	// Release mutex for accessing m_Aircrafts
}
