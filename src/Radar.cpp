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

Radar::~Radar() {
	// TODO Auto-generated destructor stub
}


void Radar::CollisionPrediction(int period) {
	cout << "Prediction for the next " << period << " seconds." << endl;

	// If you detect a collision within the period send a message to the user via COMMUNICATIONS
}

void PredictSeparationViolation(const Aircraft ac1, const Aircraft ac2, int time = PREDICTION_WINDOW) {

	if (abs(ac1.cur_pos.pz - ac2.cur_pos.pz) < MIN_SEP_Z) {
		//if (abs(sqrt(pow(ac1.cur_pos.px - ac2.cur_pos.px,2.0f) + pow(ac1.cur_pos.py - ac2.cur_pos.py,2.0f))) < MIN_SEP_X) {
		if (abs(ac1.cur_pos.px - ac2.cur_pos.px) < MIN_SEP_X || abs(ac1.cur_pos.py - ac2.cur_pos.py) < MIN_SEP_Y) {
			cout << "*********** CRASH RISK ***********" << endl;
			cout << ac1.a_id << " - " << ac2.a_id << " ARE WITHIN UNSAFE DISTANCE NEAR (" << ac1.cur_pos.px << "," << ac1.cur_pos.py << ")" << endl;
			cout << "*********** CRASH RISK ***********" << endl;
		}
		else {
		// There are four cases:
		// 1) Neither aircraft is moving (perhaps they are hovercrafts or helicopters)
		//
		// 2a) Either Aircraft moves in only 1 dimension (trajectory is perpendicular to either x axis or y axis)
		//     We must compare them in a unique manner as their slopes will be 0 or infinity.
		//
		// 2b) BOTH Aircraft are moving in 1 dimension and they are perpendicular to each other
		//     (i.e. AC1 vx == 0, vy != 0    and    AC2 vx !=0, vy == 0)
		//
		// 3) Both Aircraft are on parallel trajectories
		//    We need only concern ourselves with their proximity along the perpendicular line between their trajectories
		//    We must also evaluate their speed to determine if and when they will come together
		//    They could potentially be 1-dimensional lines, be careful of divide by 0 and divide by infinity
		//
		// 4) They are on trajectories that intersect
		//    The intersection may occur behind the path of either or both aircraft
		//    As long as the point of intersection is is ahead of at least one aircraft we must consider it
		//    Break the detection process into two steps
		//    First: compare the function of both aircrafts X position with respect to time
		//    Setting one equal to the other find the point of intersection
		//    Setting one equal to (the other + minimum X separation distance) find a point at which a potential violation could occur
		//    Setting that same one to (the other - minimum X separation distance) finds the other point
		//    These two points represent two times t1 and t2 between which the minimum separation in the x axis is violated
		//    Repeat the process for the y dimension
		//    Compare the time interval for the violation of x and y, any overlap indicates a violation
		//    The lower bound of the higher interval is the time of the violation
		//    i.e if [t1,t2] and [t3,t4] overlap and t3>t1 then the violation occurs at t3

			DoubleLine line1(ac1.cur_pos.px, ac1.cur_pos.py, ac1.cur_vel.vx, ac1.cur_vel.vy);
			DoubleLine line2(ac2.cur_pos.px, ac2.cur_pos.py, ac2.cur_vel.vx, ac2.cur_vel.vy);
			Intersection intersection{-1.0f, -1.0f, -1.0f}; // Initialize values to "No violation" flag

			if (ac1.cur_vel.vx == 0 && ac1.cur_vel.vy == 0 && ac2.cur_vel.vx == 0 && ac2.cur_vel.vy == 0) {
				// Case #1: Stationary aircraft
				// They aren't moving, we're done. Maybe one day consider the possibility of two floating helicopters

				//cout << "CASE#1 ALERT Stationary aircraft" << endl; // FOR DEBUGGING
			}
			else if ((line1.vx * line1.vy == 0.0f || line2.vx * line2.vy == 0.0f) && !line1.parallel(line2)) {
				// Case #2: At least one of the aircraft is on a trajectory perpendicular to the X or y axis

				intersection =  line1.findViolationPoint(line2, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);

				/*// First check if the trajectories are perpendicular to each other (we already determined they are not parallel)
				if (line1.vx * line1.vy == 0.0f && line2.vx * line2.vy == 0.0f) {
					// Case #2b: 1 Dimensional trajectories (perpendicular to each other)
					// There is only one point at which these two aircraft could intersect.
					// Find the interval in which each Aircraft is within minimum separation distance of that point
					// If the intervals overlap, we have a violation

					// Figure out which line is fixed on which axis
					if (line1.vx == 0) { // implies line1.vy != 0, line2.vx !=0, line2.vy == 0

						intersection = line1.findPerpendicularViolation(line2, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);
					}
					else { // implies line1.vx != 0, line1.vy == 0, line2.vx ==0, line2.vy != 0
						intersection = line2.findPerpendicularViolation(line1, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);
					}
					if (intersection.t >= 0.0f) { // FOR DEBUGGING
						cout << "CASE#2b ALERT Perpendicular one dimensional trajectories" << endl;
					}
				}
				else {
					// Case #2a: Exactly one of the aircraft is on a trajectory perpendicular to the X or y axis
					if (line1.vx == 0 || line1.vy == 0) { // Aircraft 1 has a 1 dimensional trajectory
						intersection = line1.find1DimensionalViolation(line2, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);
					}
					else { // Aircraft 2 has a 1 dimensional trajectory
						intersection = line2.find1DimensionalViolation(line1, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);
					}
					if (intersection.t >= 0.0f) { // FOR DEBUGGING
						cout << "ALERT CASE#2a One dimensional and two dimensional trajectories" << endl;
					}
				}*/
			}
			else if (line1.parallel(line2)) {
				// Case #3: Parallel trajectories

				// Are our lines within minimum range of eachother?

				// Are we getting closer with each tick?

				// Figure out the speed at which each is moving along the hypotenuse

				// TBD!



				// ***Note: If aircraft are moving extremely fast this algorithm may fail to detect the violation
				// distance between two points is sqrt( (x1-x2)^2 + (y1-y2)^2 )
				float cur_dist = abs(sqrt(pow(line1.px - line2.px,2.0f) + pow(line1.py - line2.py,2.0f)));
				float next_dist = abs(sqrt(pow((line1.px+line1.vx) - (line2.px+line2.vx),2.0f) + pow((line1.py+line1.vy) - (line2.py+line2.vy),2.0f)));
				float closingSpeed = cur_dist - next_dist;
				if (closingSpeed > 0.0f) { // If they are getting closer together on the next "tick"
					float ix, iy, it;

					it = (cur_dist - MIN_SEP_X) / closingSpeed;
					ix = line1.px + it * line1.vx;
					iy = line1.py + it * line1.vy;

					if (it <= 180.0f) {
						intersection = {ix, iy, it};
					}
				}
				// Else they are getting father apart. We are done.

				/*if (intersection.t >= 0.0f) { // FOR DEBUGGING
					cout << "Cur:" << cur_dist << " Nxt: " << next_dist << " Cls:" << closingSpeed << endl;
					cout << "CASE#3 ALERT Parallel trajectories" << endl;
				}*/
			}
			else {
				// Case #4: Intersecting 2-dimensional lines
				intersection =  line1.findViolationPoint(line2, PREDICTION_WINDOW, MIN_SEP_X, MIN_SEP_Y);
				/*if (intersection.t >= 0.0f) { // FOR DEBUGGING
					cout << "CASE#4 ALERT Intersecting two-dimensional trajectories" << endl;
				}*/
			}
			if (intersection.t >= 0.0f) { // We are only concerned with intersections that occur at future points in time
				cout << ac1.a_id << " - " << ac2.a_id << " violate separation near (" << intersection.x << "," << intersection.y << ") in " << intersection.t << " seconds." << endl;
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

		//cout << "Radar time: " << m_Time << endl; // FOR DEBUGGING

		lock_guard<mutex> aircraftlock(m_AircraftMutex);
		m_Aircrafts = m_Airspace.Scan();

		//cout << "Aircraft count: " << m_Aircrafts.size() << endl; // FOR DEBUGGING

		// N^2 solution, not great.
		for (int i = 0; i < m_Aircrafts.size(); i++) {
			for (int j = i+1; j < m_Aircrafts.size(); j++) {
				PredictSeparationViolation(m_Aircrafts[i], m_Aircrafts[j]);
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
		{ // Fake scope for lock_guard to die inside
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
