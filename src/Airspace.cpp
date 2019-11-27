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
		m_A_Dataset.emplace_back(next_unique_id++, data[i], pos, velo, data[i+7]);
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
	while (m_SystemOnline)
	{
		// Wait for a unit of time to pass
		unique_lock<mutex> timelock(m_TimeMutex);
		//m_Cond_Time.wait(timelock);
		m_Cond_Time.wait(timelock,
			[&current_time = m_Time, &lt = last_time]
			 { return (current_time > lt); });
			// To prevent spurious wakeup make sure time has actually passed

		last_time = m_Time;


		lock_guard<mutex> activelock(m_ActiveMutex);
		{
			lock_guard<mutex> broadlock(m_BroadMutex);
			lock_guard<mutex> msglock(m_MsgMutex);
			// Let all of the aircraft respond to messages and fly one unit of Velocity
			for (auto it = m_Aircrafts.begin(); it != m_Aircrafts.end(); ) {
				if (it->cur_pos.px < 0 || it->cur_pos.px > 105*5280 || it->cur_pos.py < 0 || it->cur_pos.py > 105*5280) {
					it = m_Aircrafts.erase(it); // If it leaves our airspace, erase it from the Active Aircraft list
					// Erase returns an iterator pointing to the next element of our list
				} else {
					for (auto mit = m_Messages.begin(); mit != m_Messages.end(); ) {
						if (mit->dest_id == it->a_id) {
							// do what the message says
							switch (mit->type) {
								case 1: { // 1 = Report position and velocity
									if (it->a_id == -1) {
										it->a_id = next_unique_id++;
									}
									lock_guard<mutex> respmutex(m_ResponseMutex);
									m_Responses.push_back(it->Response());
									break;
								}
								case 2: { // 2 = Enter holding pattern
									if (!it->holding) {
										it->StartHolding();
									}
									break;
								}
								case 3: { // 3 = Exit holding pattern
									if (it->holding) {
										it->StopHolding();
										it->holdingTimer = PREDICTION_WINDOW;
									}
									break;
								}
								case 4: { // 4 = Change velocity
									it->ChangeVelocity(mit->vel);
									break;
								}
								case 5: { // 5 = Change elevation
									it->ChangeElevation(mit->elevation);
									break;
								}
								default: break;
							}
							// delete the message
							mit = m_Messages.erase(mit);
						}
						else {
							mit++;
						}
					}
					for (auto bit = m_Broadcasts.begin(); bit != m_Broadcasts.end(); ) {
						// do what the broadcast says
						switch (bit->type) {
							case 6: { // 6 = Report position and velocity
								if (it->a_id == -1) {
									it->a_id = next_unique_id++;
								}
								lock_guard<mutex> respmutex(m_ResponseMutex);
								m_Responses.push_back(it->Response());
								break;
							}
							case 7: { // 7 = Enter holding pattern
								if (!it->holding) {
									it->StartHolding();
								}
								break;
							}
							case 8: { // 8 = Exit holding pattern
								if (it->holding) {
									it->StopHolding();
									it->holdingTimer = PREDICTION_WINDOW;
								}
								break;
							}
							default: break;
						}
						bit++;
					}
					it->fly();
					it++;
				}
			}
			// delete all broadcasts
			m_Broadcasts.clear();
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

// We sort aircraft by entry time, lowest (earliest) first
bool CompareAircraft(Aircraft a1, Aircraft a2) {
	return a1.a_et < a2.a_et;
}

void Airspace::AddAircraft() {
	int entry_time = 0;
	cout << "Specify the aircraft parameters" << endl <<
			"(AID, Position, Velocity, Entry Time)" << endl;
	Aircraft newAc;
	newAc.s_id = next_unique_id++;
	cout << endl << "\tEnter Aircraft ID: ";
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
	cin >> newAc.a_et;

	newAc.grid_pos.px = ceil((float)(newAc.cur_pos.px/5280)/3);
	newAc.grid_pos.py = ceil((float)(newAc.cur_pos.py/5280)/3);
	newAc.grid_pos.pz = newAc.cur_pos.pz;

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
}

// This function can only be called when simulation time has been stopped
bool Airspace::ChangeAircraft(Aircraft& ac) {
	bool acDeleted = false;

	char input = '\0';
	const char edit = 'E';
	const char del = 'D';
	const char conf = 'C';
	const char exit = 'B'; // The last option of our main menu
	while (!acDeleted && input != exit) {
		cout << "SID: " << ac.s_id << " A";
		ac.PrintMembers();

		cout << endl << "Choose action for this aircraft" << endl << endl;

		cout << "\t" << edit << ")Edit" << endl;
		cout << "\t" << del << ")Delete" << endl;
		//cout << "\t1)Add an aircraft" << endl;
		cout << endl << "\t" << exit << ")Back to previous menu" << endl;

		cin >> input;

		switch (input) {
			case exit: break;
			case edit: {// Edit
				cout << endl << "\tNew Aircraft ID: ";
				cin >> ac.a_id;
				cout << endl << "\tNew X Position: ";
				cin >> ac.cur_pos.px;
				cout << endl << "\tNew Y Position: ";
				cin >> ac.cur_pos.py;
				cout << endl << "\tNew Z Position: ";
				cin >> ac.cur_pos.pz;
				cout << endl << "\tNew X Velocity: ";
				cin >> ac.cur_vel.vx;
				cout << endl << "\tNew Y Velocity: ";
				cin >> ac.cur_vel.vy;
				cout << endl << "\tNew Z Velocity: ";
				cin >> ac.cur_vel.vz;

				ac.grid_pos.px = ceil((float)(ac.cur_pos.px/5280)/3);
				ac.grid_pos.py = ceil((float)(ac.cur_pos.py/5280)/3);
				ac.grid_pos.pz = ac.cur_pos.pz;

				bool editComplete = false;
				{
					lock_guard<mutex> activelock(m_ActiveMutex);
					auto it = find_if(m_Aircrafts.begin(), m_Aircrafts.end(), [&sid = ac.s_id](const Aircraft& ac_2) {return ac_2.s_id == sid;});

					if (it != m_Aircrafts.end())
					{
					  // found element. 'it' is an iterator to the first matching element
					  *it = ac;
					  editComplete = true;
					}
				}

				if (!editComplete) {
					{
						lock_guard<mutex> datalock(m_DataMutex);
						auto it = find_if(m_A_Dataset.begin(), m_A_Dataset.end(), [&sid= ac.s_id](const Aircraft& ac_2) {return ac_2.s_id == sid;});

						if (it != m_A_Dataset.end())
						{
						  // found element. 'it' is an iterator to the first matching element
						  *it = ac;
						  // Always sort the Data Set by entry time
						  sort(m_A_Dataset.begin(), m_A_Dataset.end(), CompareAircraft);
						}
					}
				}

				break;
			}
			case del: { // Delete
				char confirm = '\0';

				cout << "CONFIRM DELETION" << endl;
				cout << "\tThis action cannot be undone!" << endl;
				cout << "\tEnter '" << conf << "' to confirm deletion." << endl;
				cout << "\tEnter 'B' to cancel." << endl;

				cin >> confirm;

				if (confirm == conf) {
					lock_guard<mutex> datalock(m_DataMutex);
					auto it = find_if(m_A_Dataset.begin(), m_A_Dataset.end(), [&sid= ac.s_id](const Aircraft& ac_2) {return ac_2.s_id == sid;});

					if (it != m_A_Dataset.end())
					{
						m_A_Dataset.erase(it);
						/*auto index = std::distance(m_A_Dataset.begin(), it);
						m_A_Dataset[index].erase();*/
						acDeleted = true;
					}
				}
				break;
			}
			default: break;
		}
	}
	return acDeleted;
}

vector<Aircraft> Airspace::Scan() {
	lock_guard<mutex> activelock(m_ActiveMutex);
	return m_Aircrafts;
}

vector<Aircraft> Airspace::getActiveAircraft() {
	lock_guard<mutex> activelock(m_ActiveMutex);
	return m_Aircrafts;
}

vector<Aircraft> Airspace::getAircraftDataSet() {
	lock_guard<mutex> datalock(m_DataMutex);
	return m_A_Dataset;
}

void Airspace::SendMessage(Message msg) {
	if (m_SystemOnline) {
		lock_guard<mutex> msglock(m_MsgMutex);
		m_Messages.push_back(msg);
	}
}

void Airspace::Broadcast(Message brdcst) {
	if (m_SystemOnline) {
		lock_guard<mutex> brdcstlock(m_BroadMutex);
		m_Broadcasts.push_back(brdcst);
	}
}

vector<string> Airspace::GetResponses() {
	lock_guard<mutex> respmutex(m_ResponseMutex);
	vector<string> returnVector = m_Responses;
	m_Responses.clear();
	return returnVector;
}
