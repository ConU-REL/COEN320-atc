#pragma once


#include <vector>
#include <iostream>
#include <map>
#include "Aircraft.h"

class Airspace {

public:
	static Airspace& getInstance();
	std::vector<Aircraft> Scan();

	bool AdvanceTime();
	void ChangeAircraft(int, Aircraft);

private:
	int m_Time = 0;
	int m_Next_Event;
	std::vector<Aircraft> m_TestData;
	std::vector<Aircraft> m_Aircrafts;
	//std::vector<Environment> m_Environment;

	void ProcessEvents();

	Airspace();
	virtual ~Airspace();
};
