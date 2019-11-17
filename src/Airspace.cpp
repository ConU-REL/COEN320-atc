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

	for (size_t i = 0; i < size; i += 8) {
		Velocity velo{ data[i+1], data[i+2], data[i+3] };
		Position pos{ data[i+4], data[i+5], data[i+6] };
		m_Aircrafts.emplace_back(data[i], pos, velo);
		m_LastPings[data[i]] = data[i+7] - 15; // TBD - How do we handle Unknown aircraft?
	}

	cout << "Aircraft 0 has entry point: "<< m_LastPings[0] << endl;

	cout << "Aircraft -1 has entry point: "<< m_LastPings[-1] << endl;
}

Airspace::~Airspace() {
	// TODO Auto-generated destructor stub
}

void Airspace::CollisionPrediction(int period) {
	cout << "Prediction for the next " << period << " seconds." << endl;
}
