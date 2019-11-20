#include "Airspace.h"

#include "TestCase.h"

#include <iostream>
#include <cstring>

using namespace std;

Airspace& Airspace::getInstance() {
	static Airspace _instance;
	return _instance;
}

Airspace::Airspace() {
	// Gather and structure data set we will initially work with
	TestCase tc;
	auto const& data = tc.airplane_schedule;
	const size_t size = sizeof(data) / sizeof(*data);
	static_assert(size % 8 == 0, "Bad data.");

	m_Next_Event = data[7]; // Use first entry time as the first event.

	// Aircraft data is supplied as such:
	// Index 0: Aircraft ID, Index 7: Entry time
	// Indices 1,2,3: Velocity x,y,z (respectively)
	// Indices 4,5,6: Position x,y,z (respectively)
	for (size_t i = 0; i < size; i += 8) {
		Velocity velo{ data[i+1], data[i+2], data[i+3] };
		Position pos{ data[i+4], data[i+5], data[i+6] };
		lock_guard<mutex> datalock(m_DataMutex); // This shouldn't technically be necessary at this point
		m_A_Dataset.emplace_back(data[i], pos, velo, data[i+7]);
		lock_guard<mutex> timelock(m_TimeMutex); // This shouldn't technically be necessary at this point
		if (data[i+7] < m_Next_Event) {
			m_Next_Event = data[i+7];
		}
	}

	// Create the thread that will simulate the world
	m_ProcessingThread = new thread{ &Airspace::ProcessMovement, this };
	cout << "Airspace instantiated" << endl;
}

// Runs in a separate thread. Tracks position of aircraft with the highest degree of accuracy
void Airspace::ProcessMovement() {
	int last_time = 0;
	while (m_SimulationRunning)
	{
		// Wait for a unit of time to pass
		unique_lock<mutex> timelock(m_TimeMutex);
		//m_Cond_Time.wait(timelock);
		m_Cond_Time.wait(timelock,
			[&current_time = m_Time, &lt = last_time]
			 { return (current_time > lt); });
			// To prevent spurious wakeup make sure time has actually passed

		last_time = m_Time;

		// Let all of the aircraft fly one unit of Velocity
		lock_guard<mutex> activelock(m_ActiveMutex);
		/*for (int i = 0; i < m_Aircrafts.size(); i++) {
			m_Aircrafts[i].fly();
		}*/
		for (Aircraft& ac : m_Aircrafts) {
			ac.fly();
		}
		//cout << "FLYING! Elapsed time: " << m_Time << endl;

		// TBD add environment movement

		if (m_Next_Event <= m_Time) {
			// Determine how many aircraft in the data set have entered our system
			int i=0;
			lock_guard<mutex> datalock(m_DataMutex);
			for (Aircraft& ac : m_A_Dataset) {
				if (ac.a_et <= m_Time) { // If their entry time is now or in the past
					i++;
				}
				else {
					m_Next_Event = ac.a_et; // Mark the next aircraft entry time
					break; // The data set is always sorted by entry time, stop if entry time is in the future
				}
			}

			// The data set is always sorted by entry time, so the first i rows should be added
			if ( i > 0) { // If we have at least one aircraft to add to the active set
				//lock_guard<mutex> activelock(m_ActiveMutex); // we already have this mutex from earlier
				m_Aircrafts.insert(m_Aircrafts.end(),m_A_Dataset.begin(),m_A_Dataset.begin()+i);
				m_A_Dataset.erase(m_A_Dataset.begin(),m_A_Dataset.begin()+i);
			}

			// TBD add environment
		}
	}
}

bool Airspace::AdvanceTime(int seconds) {
	for (int i = 1; i <= seconds; i++) {
		{
		lock_guard<mutex> timelock(m_TimeMutex);
		m_Time++;
		}
		m_Cond_Time.notify_one(); // Let ProcessMovement run
	}
	return true;
}

bool CompareAircraft(Aircraft a1, Aircraft a2) {
	return a1.a_et < a2.a_et;
}

// This function can only be called when simulation time has been stopped
void Airspace::ChangeAircraft() {
	int input = 0;
	const int exit = 4; // The last option of our main menu
	while (input != exit) {
		cout << "Choose action:" << endl << endl;
		cout << "\t1)Add an aircraft" << endl;
		cout << "\t2)Delete an aircraft" << endl;
		cout << "\t3)Edit an aircraft" << endl;
		cout << endl << "\t" << exit << ")Cancel" << endl;

		switch (input) {
			case exit: break;
			case 1: { // Add
				int entry_time = 0;
				cout << "Specify the aircraft parameters" << endl <<
						"(ID, Position, Velocity, Entry Time)" << endl;
				Aircraft newAc;
				cout << endl << "\tEnter aircraft Id: ";
				cin >> newAc.a_id;
				cout << endl << "\tEnter X Position: ";
				cin >> newAc.cur_pos.px;
				cout << endl << "\tEnter Y Position: ";
				cin >> newAc.cur_pos.py;
				cout << endl << "\tEnter Z Position: ";
				cin >> newAc.cur_pos.pz;
				cout << endl << "\tEnter X Velocity: ";
				cin >> newAc.cur_vel.vx;
				cout << endl << "\tEnter Y Velocity: ";
				cin >> newAc.cur_vel.vy;
				cout << endl << "\tEnter Z Velocity: ";
				cin >> newAc.cur_vel.vz;
				cout << endl << "\tEnter aircraft Entry Time: ";
				cin >> entry_time;
				newAc.a_et = entry_time;

				lock_guard<mutex> timelock(m_TimeMutex);
				if (entry_time > m_Time) { // If this aircraft is entering our system at a later time
					lock_guard<mutex> datalock(m_DataMutex);
					m_A_Dataset.push_back(newAc);
					// Always sort the Data Set by entry time
					sort(m_A_Dataset.begin(), m_A_Dataset.end(), CompareAircraft);
				}
				else { // The aircraft is appearing in our system immediately
					lock_guard<mutex> activelock(m_ActiveMutex);
					m_Aircrafts.push_back(newAc);
				}
				break;
			}
			case 2: { // Delete
				cout << "TBD DELETE BY ID" << endl;
				cout << "ASK USER IF THEY WANT TO DELETE FROM FUTURE OR ACTIVE AIRCRAFT" << endl;
				cout << "LIST THOSE AIRCRAFT" << endl;
				cout << "PROMPT USER FOR SYSTEM ID OF AIRCRAFT" << endl;
				cout << "DELETE THAT AIRCRAFT" << endl;

					//m_A_Dataset.erase();
					break;
			}
			case 3: {// Edit
				cout << "TBD SIMILAR TO ADD" << endl;
					break;
			}
			default: break;
		}
	}
}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}

vector<Aircraft> Airspace::Scan() {
	lock_guard<mutex> activelock(m_ActiveMutex);
	return m_Aircrafts;
}
