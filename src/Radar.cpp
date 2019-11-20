#include "Radar.h"
#include "Line.h"
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

bool SeparationViolation(const Aircraft ac1, const Aircraft ac2, int time = PREDICTION_WINDOW) {
	bool violation = false;

	// The idea is as follows:
	// Treat Aircraft 2's trajectory as an ordinary line
	// Construct a rectangle centered around Aircraft 1's current position
	// The dimensions of this rectangle are defined by the minimum X and Y separation required between aircraft
	// Starting from each of the four corners of this rectangle, plot trajectories parallel to that of Aircraft 1
	// If Aircraft 2 intersects with any of those four trajectories we must check at what time it occurs
	// If it occurs at a time when both Aircraft occupy that space, the minimum separation has been violated
	// When multiple lines are intersected the earliest time is the first violation and should be reported
	/*
	                                  Aircraft 2 (Now)
	                                         X
	    2-Dimensional top-down view           \
	                                           \
	                                            \
	                    _   ___________ _ _ _ _ _\_ _ _ _ _ _ _ ___________ _ _ _ _ _ _ _ _ _
	                   |    |         |           \             |         |
	                   |    |         |            \            |         |
	2x Min separation <	    |Aircraft1|----->------>\---->----->|Aircraft1|
	                   |    |  (Now)  |              \          |(Future) |
	                   |_   |_________| _ _ _ _ _ _ _ \ _ _ _ _ |_________| _ _ _ _ _ _ _ _ _
                                                       \
                                                        O
                                                 Aircraft 2 (Future)
	*/

	// Exceptional cases:
	// 1) Both Aircraft move in only 1 dimension and they are the opposite dimensions
	//    (example: AC1 vx == 0, vy != 0    and    AC2 vx !=0, vy == 0)
	//
	// 2) Both Aircraft are on parallel trajectories
	//
	// 3) Any situation where the angle of Aircraft 2 is so small (or difference in speed is so large)
	//    that the two aircraft pass through each other long before the point of intersection

	/*
		               ___________ _ _ _ _ _ _ _ _ _ _ _ _ ___________ _ _ _ _ _ _ _ _ _
		 Aircraft 2    |         |                         |         |                      Aircraft 2
			O----------------------------------------------------------------------------------X
	     (Future)      |Aircraft1|----->------>----->----->|Aircraft1|                        (Now)
				       |  (Now)  |                         |(Future) |
				       |_________| _ _ _ _ _ _ _ _ _ _ _ _ |_________| _ _ _ _ _ _ _ _ _
	*/

	if (abs(ac1.cur_pos.pz - ac2.cur_pos.pz) < MIN_SEP_Z) { // Aircraft violate z distance
		Line line1(ac1.cur_pos.px, ac1.cur_pos.py, ac1.cur_vel.vx, ac1.cur_vel.vy);
		Line line2(ac2.cur_pos.px, ac2.cur_pos.py, ac2.cur_vel.vx, ac2.cur_vel.vy);

		if (ac1.cur_vel.vx == 0 && ac1.cur_vel.vy == 0 && ac2.cur_vel.vx == 0 && ac2.cur_vel.vy == 0) {
			// They aren't moving, we're done. Maybe one day consider the possibility of two floating helicopters
		}
		else if (line1.vx * line1.vy == 0.0f && line2.vx * line2.vy == 0.0f && !line1.parallel(line2)) {
			// Exceptional case #1
			// There is only one point at which these two aircraft could intersect.
			// Find the interval in which each Aircraft is within minimum separation distance of that point
			// If the intervals overlap, we have a violation

			float ix, iy;

			// Figure out which line is fixed on which axis
			if (line1.vx == 0) { // implies line1.vy != 0, line2.vx !=0, line2.vy == 0
				ix = line1.px;
				iy = line2.py;

				float xDist = abs(ix - line2.px);

				float t1 = (xDist - MIN_SEP_X) / abs(line2.vx);
				float t2 = (xDist + MIN_SEP_X) / abs(line2.vx);

				float yDist = abs(iy - line1.py);

				float t3 = (yDist - MIN_SEP_Y) / abs(line1.vy);
				float t4 = (yDist + MIN_SEP_Y) / abs(line1.vy);

				if (t2 >= 0.0f || t4 >= 0.0f) { // TBD This reports a few negative values
				//if ((t1 >= 0.0f && t2) >= 0.0f || (t3 >= 0.0f && t4 >= 0.0f)) { // This never stops reporting negative values
					if (max(t1,t3) - min(t2,t4) <= 0.0f) { // Check if the time intervals overlap
						cout << ac1.a_id << " - " << ac2.a_id << " violate separation at (" << ix << "," << iy << ")" << endl;
						cout << "Violation will occur in " << abs(ceil(max(t1,t3))) << " seconds." << endl;
						violation = true;
					}
				}
			}
			else { // implies line1.vx != 0, line1.vy == 0, line2.vx ==0, line2.vy != 0
				ix = line2.px;
				iy = line1.py;

				float xDist = ix - line1.px;

				float t1 = (xDist - MIN_SEP_X) / line1.vx;
				float t2 = (xDist + MIN_SEP_X) / line1.vx;

				float yDist = iy - line2.py;

				float t3 = (yDist - MIN_SEP_Y) / line2.vy;
				float t4 = (yDist + MIN_SEP_Y) / line2.vy;

				if (t2 >= 0.0f || t4 >= 0.0f) { // TBD This reports a few negative values
				//if ((t1 >= 0.0f && t2) >= 0.0f || (t3 >= 0.0f && t4 >= 0.0f)) { // This never stops reporting negative values
					if (max(t1,t3) - min(t2,t4) <= 0.0f) { // Check if the time intervals overlap
						cout << ac1.a_id << " - " << ac2.a_id << " violate separation at (" << ix << "," << iy << ")" << endl;
						cout << "Violation will occur in " << ceil(max(t1,t3)) << " seconds." << endl;
						violation = true;
					}
				}

			}

		}
		else if (line1.parallel(line2)) {
			// They are parallel


		}
		else {
			vector<Line> fourCorners;
			fourCorners.emplace_back(ac1.cur_pos.px+MIN_SEP_X, ac1.cur_pos.py+MIN_SEP_Y, ac1.cur_vel.vx, ac1.cur_vel.vy);
			fourCorners.emplace_back(ac1.cur_pos.px+MIN_SEP_X, ac1.cur_pos.py-MIN_SEP_Y, ac1.cur_vel.vx, ac1.cur_vel.vy);
			fourCorners.emplace_back(ac1.cur_pos.px-MIN_SEP_X, ac1.cur_pos.py+MIN_SEP_Y, ac1.cur_vel.vx, ac1.cur_vel.vy);
			fourCorners.emplace_back(ac1.cur_pos.px-MIN_SEP_X, ac1.cur_pos.py-MIN_SEP_Y, ac1.cur_vel.vx, ac1.cur_vel.vy);

			float ix_min, iy_min;
			float t1min = numeric_limits<float>::max();
			float t2min = numeric_limits<float>::max();

			for (Line& l : fourCorners) {
				float ix = line2.findXIntersectPoint(l); // point of intersection
				float iy = line2.m * ix + line2.b;

				float t1 = (ix - line2.px) / line2.vx;
				float t2 = (ix - l.px) / l.vx;

				if (t1 >= 0.0f && (t1 - t2) <= 1.0f) { // They reach that point at the same time
					if ( t1 < t1min) {
						t1min = t1;
						t2min = t2;
						ix_min = ix;
						iy_min = iy;
					}
				}
			}

			if (t1min != numeric_limits<float>::max()) {
				cout << ac1.a_id << " - " << ac2.a_id << " violate separation at (" << ix_min << "," << iy_min << ")" << endl;
				cout << ac1.a_id << " reaches this point in " << t1min << " seconds." << endl;
				cout << ac2.a_id << " reaches this point in " << t2min << " seconds." << endl;
			}
		}
	}
	return violation;
}

	/*
				float a1x1, a1y1, a1x2, a1y2, a2x1, a2y1, a2x2, a2y2; // start and end points of each aircraft
				float dx, dy; // run and rise for calculating slope
				float a1m, a2m; // slope of each aircraft
				float a1c, a2c; // y intercept of each aircraft
				a1x1 = ac1.cur_pos.px;
				a1y1 = ac1.cur_pos.py;
				a1x2 = a1x1 + ac1.cur_vel.vx*time;
				a1y2 = a1y1 + ac1.cur_vel.vy*time;

				dx = a1x2 - a1x1;
				dy = a1y2 - a1y1;

				a1m = dy / dx;
				// y = mx + c
				// y - mx = c
				// c = y - mx
				a1c = a1y1 - a1m * a1x1;
				//a1c = a1y2 - a1m * a1x2; // also works

				a2x1 = ac2.cur_pos.px;
				a2y1 = ac2.cur_pos.py;
				a2x2 = a2x1 + ac2.cur_vel.vx*time;
				a2y2 = a2y1 + ac2.cur_vel.vy*time;

				dx = a2x2 - a2x1;
				dy = a2y2 - a2y1;

				a2m = dy / dx;
				a2c = a2y1 - a2m * a2x1;
				//c2 = y2 - m2 * x2; // also works

				if ((a1m - a2m) != 0) { // The trajectories are not parallel
					float ix; // point of intersection
					ix = (a2c-a1c) / (a1m-a2m);

					float iy;
					iy = a1m * ix + a1c;
					cout << ac1.a_id << " - " << ac2.a_id << " intersect at (" << ix << "," << iy << ")" << endl;


					float a1t, a2t; // Time at which each aircraft reaches the intersection point

					a1t = (ix - ac1.cur_pos.px); // Get the numerator alone first, for use in a comparison
					a2t = (ix - ac2.cur_pos.px); // Get the numerator alone first, for use in a comparison

					// Check if the intersection point is in the forward direction of at least one aircraft
					// (only need one in the case of one aircraft overtaking the other)
					cout << (SameSign(a1t - a1x1, a1x2-a1x1) || SameSign(a2t - a2x1, a2x2-a2x1)) << endl;
					if (SameSign(a1t - a1x1, a1x2-a1x1) || SameSign(a2t - a2x1, a2x2-a2x1)) {
						a1t = a1t / ac1.cur_vel.vx; // Get the actual time of intersection, will likely not be an exact integer
						a2t = a2t / ac2.cur_vel.vx;

						int t1a, t1b, t2a, t2b;

						t1a = floor(a1t);
						t2a = floor(a2t);
						t1b = ceil(a1t);
						t2b = ceil(a2t);

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
				}*/

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
					// TBD Check if they are violating minimum separation already! else call the function
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
