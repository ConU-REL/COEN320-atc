#include "Radar.h"
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

Radar::~Radar() {
	// TODO Auto-generated destructor stub
}


void Radar::CollisionPrediction(int period) {
	cout << "Prediction for the next " << period << " seconds." << endl;

	// If you detect a collision within the period send a message to the user via COMMUNICATIONS
}

bool SameSign(float a, float b) {
	return a*b >= 0.0f;
}

bool SeparationViolation(const Aircraft ac1, const Aircraft ac2, int time = PREDICTION_WINDOW) {
	bool violation = false;

	float x1, y1, x2, y2; // start and end points of each aircraft
	float dx, dy; // run and rise for calculating slope
	float m1, m2; // slope
	float c1, c2; // y intercept
	x1 = ac1.cur_pos.px;
	y1 = ac1.cur_pos.py;
	x2 = x1 + ac1.cur_vel.vx;
	y2 = y1 + ac1.cur_vel.vy;

	dx = x2 - x1;
	dy = y2 - y1;

	m1 = dy / dx;
	// y = mx + c
	// y - mx = c
	// c = y - mx
	c1 = y1 - m1 * x1;
	//c1 = y2 - m1 * x2; // also works

	x1 = ac2.cur_pos.px;
	y1 = ac2.cur_pos.py;
	x2 = x2 + ac1.cur_vel.vx;
	y2 = y2 + ac1.cur_vel.vy;

	dx = x2 - x1;
	dy = y2 - y1;

	m2 = dy / dx;
	c2 = y1 - m1 * x1;
	//c2 = y2 - m2 * x2; // also works

	if ((m1 - m2) != 0) { // Lines intersect
		if (abs(ac1.cur_pos.pz - ac2.cur_pos.pz) < MIN_SEP_Z) { // Intersection is within minimum z separation
			float ix; // point of intersection
			ix = (c2-c1) / (m1-m2);
			//float iy;
			//iy = m1 * ix + c1;

			float t1, t2;

			t1 = (ix - ac1.cur_pos.px);
			t2 = (ix - ac2.cur_pos.px);

			// Check if the intersection point is behind either aircraft
			if (SameSign(t1, ac1.cur_vel.vx) && SameSign(t2, ac2.cur_vel.vx)) {
				t1 = t1 / ac1.cur_vel.vx;
				t2 = t2 / ac2.cur_vel.vx;

				int t1a, t1b, t2a, t2b;

				t1a = floor(t1);
				t2a = floor(t2);
				t1b = ceil(t1);
				t2b = ceil(t2);

				if (t1b <= time || t2b <= time || t1a <= time || t2a <= time) {
					if (t1a == t2a) {
						violation = true;
						cout << ac1.a_id << " - " << ac2.a_id << " minimum separation violation projected in " << t1a << " seconds" << endl;
					}
					else if (t1a == t2b) {
						violation = true;
						cout << ac1.a_id << " - " << ac2.a_id << " minimum separation violation projected in " << t1a << " seconds" << endl;
					}
					else if (t1b == t2a) {
						violation = true;
						cout << ac1.a_id << " - " << ac2.a_id << " minimum separation violation projected in " << t1b << " seconds" << endl;
					}
				}

			}

		}
	}

	return violation;
}

void Radar::ProcessTime() {
	int last_time = 0;
	while (m_SimulationRunning)
		{
			// Wait for a unit of time to pass
			unique_lock<mutex> timelock(m_TimeMutex);
			//m_Cond_ScanTime.wait(timelock);
			m_Cond_ScanTime.wait(timelock,
				[&current_time = m_Time, &lt = last_time]
				 { return ((current_time > lt)&&(current_time % RADAR_INTERVAL == 0)); });
				// To prevent spurious wakeup make sure it is actually scan time
			last_time = m_Time;
			//cout << "Radar time: " << m_Time << endl;

			lock_guard<mutex> aircraftlock(m_AircraftMutex);
			m_Aircrafts = m_Airspace.Scan();

			for (int i = 0; i < m_Aircrafts.size(); i++) {
				for (int j = i+1; j < m_Aircrafts.size(); j++) {
					if (SeparationViolation(m_Aircrafts[i], m_Aircrafts[j])) {
						// Send a message to the user via COMMUNICATIONS
					}
				}
			}

			m_Cond_ReportReady.notify_one();

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
