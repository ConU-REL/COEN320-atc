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
	cout << "Airspace instantiated" << endl;

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
		m_TestData.emplace_back(data[i], pos, velo);
		if (data[i+7] < m_Next_Event) {
			m_Next_Event = data[i+7];
		}
	}
}

// Runs in a separate thread. Tracks position of aircraft with the highest degree of accuracy
void Airspace::ProcessEvents() {

	// OBTAIN Condvar and mutex for accessing m_Changes
	// If m_Changes is not empty
		// OBTAIN Condvar and mutex for accessing m_TestData
		// Append contents of m_Changes to m_TestData
		// RELEASE mutex for accessing m_Changes
		// Sort m_TestData by entry time (lowest to highest)
		// Set m_Next_Event to the entry_time of the first aircraft in m_TestData
		// RELEASE mutex for accessing m_TestData
	// Else RELEASE mutex for accessing m_Changes

	// If m_Next_Event >= m_Time is true
	// OBTAIN Condvar and mutex for accessing m_TestData
	// OBTAIN Condvar and mutex for accessing m_Aircrafts
		// While m_TestData[i].entry_time <= m_Time && i < m_TestData.size()
			// copy m_TestData[i++] into m_Aircrafts
	// Release mutex for accessing m_TestData
	// Release mutex for accessing m_Aircrafts
}

bool Airspace::AdvanceTime() {
	if (m_Next_Event >= ++m_Time) {
		//Notify the running thread to add/delete an aircraft/environment
	}
	return true;
}

void Airspace::ChangeAircraft(int mode, Aircraft aircraft) {
	switch (mode) {
		case 1: // Add
				break;
		case 2: // Delete
				break;
		case 3: // Edit
				break;
		default:
				break;
	}
	m_Next_Event = m_Time;
	//Notify the running thread to add/edit/delete an aircraft
}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}

vector<Aircraft> Airspace::Scan() {
	// OBTAIN mutex for accessing m_Aircrafts
	return m_TestData; // FOR TESTING ONLY, normally return m_Aircrafts
	return m_Aircrafts;
	// RELEASE mutex for accesing m_Aircrafts
}
