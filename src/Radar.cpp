#include "Radar.h"
#include "Line.h"
#include "TimeInterval.h"
#include <iostream> // FOR DEBUGGING
#include <cmath>

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

void Radar::PredictSeparationViolation(const Aircraft ac1, const Aircraft ac2, int time) {

	if (abs(ac1.cur_pos.pz - ac2.cur_pos.pz) < MIN_SEP_Z) {
		//if (abs(sqrt(pow(ac1.cur_pos.px - ac2.cur_pos.px,2.0f) + pow(ac1.cur_pos.py - ac2.cur_pos.py,2.0f))) < MIN_SEP_X) { // circular separation
		if (abs(ac1.cur_pos.px - ac2.cur_pos.px) < MIN_SEP_X && abs(ac1.cur_pos.py - ac2.cur_pos.py) < MIN_SEP_Y) { // rectangular separation
			lock_guard<mutex> warninglock(m_WarningMutex);
			m_Warnings.emplace_back("*********** CRASH RISK ***********\n" +
					to_string(ac1.a_id) + " - " + to_string(ac2.a_id) + " ARE WITHIN UNSAFE DISTANCE NEAR (" + to_string(ac1.cur_pos.px) + "," + to_string(ac1.cur_pos.py) + ")\n" +
					"*********** CRASH RISK ***********\n");
		}
		else {
			int timeToCheck;
			DoubleLine line1(ac1.cur_pos.px, ac1.cur_pos.py, ac1.cur_vel.vx, ac1.cur_vel.vy);
			DoubleLine line2(ac2.cur_pos.px, ac2.cur_pos.py, ac2.cur_vel.vx, ac2.cur_vel.vy);

			if (ac1.holding || ac2.holding) { // If an aircraft is in holding pattern we only need to check until it the holding aircraft turns
				timeToCheck = min(ac1.holdingTimer,ac2.holdingTimer);
			}
			else { // Otherwise check for the whole 180 seconds (or whatever it gets set to)
				timeToCheck = time;
			}

			Intersection intersection{-1.0f, -1.0f, -1.0f}; // Initialize values to "No violation" flag

			if (line1.parallel(line2)) { // Our algorithm doesn't work for perfectly parallel trajectories.
				line1.m -= 0.0000001f;
			}

			intersection =  line1.findViolationPoint(line2, timeToCheck, MIN_SEP_X, MIN_SEP_Y); // rectangular separation

			if (intersection.t >= 0.0f) { // We are only concerned with intersections that occur at future points in time
				lock_guard<mutex> warninglock(m_WarningMutex);
				m_Warnings.emplace_back(to_string(ac1.a_id) + " - " + to_string(ac2.a_id) + " violate separation near (" + to_string((int)intersection.x) + "," + to_string((int)intersection.y) + ") in " + to_string((int)ceil(intersection.t)) + " seconds.\n");
				//cout << ac1.a_id << " - " << ac2.a_id << " violate separation near (" << intersection.x << "," << intersection.y << ") in " << intersection.t << " seconds." << endl;
			}
		}
	}
}

void Radar::ProcessTime() {
	int last_time = 0;
	while (m_SystemOnline) {
		// Wait for a unit of time to pass
		unique_lock<mutex> timelock(m_TimeMutex);
		m_Cond_ScanTime.wait(timelock,
			[&current_time = m_Time, &lt = last_time]
			 { return ((current_time > lt)&&(current_time % RADAR_INTERVAL == 0)); });
			// To prevent spurious wakeup make sure it is actually scan time
		last_time = m_Time;

		lock_guard<mutex> aircraftlock(m_AircraftMutex);
		m_Aircrafts = m_Airspace.Scan();

		m_WarningMutex.lock();
		m_Warnings.clear();
		m_WarningMutex.unlock();

		// N^2 solution, not great.
		for (int i = 0; i < m_Aircrafts.size(); i++) {
			for (int j = i+1; j < m_Aircrafts.size(); j++) {
				PredictSeparationViolation(m_Aircrafts[i], m_Aircrafts[j]);
			}
		}
		m_Cond_ReportReady.notify_one();
	}
}

bool Radar::AdvanceTime(int seconds) {
	for (int i = 1; i <= seconds; i++) {
		{
			lock_guard<mutex> timelock(m_TimeMutex);
			m_Time++;
			if (m_Time % RADAR_INTERVAL == 0) {
				m_Cond_ScanTime.notify_one(); // Let ProcessTime run
			}
		}

	}
	return true;
}

vector<Aircraft> Radar::Report() {
	unique_lock<mutex> aircraftlock(m_AircraftMutex);
	m_Cond_ReportReady.wait(aircraftlock);
	return m_Aircrafts;
}

vector<string> Radar::Warnings() {
	lock_guard<mutex> warninglock(m_WarningMutex);
	return m_Warnings;
}
