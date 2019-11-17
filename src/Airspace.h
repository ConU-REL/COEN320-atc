#pragma once


#include <vector>
#include <iostream>
#include <map>
#include "Aircraft.h"

class Airspace {

public:
	static Airspace& getInstance();
	void CollisionPrediction(int period = 180);

private:
	int m_Time = 0;
	std::vector<Aircraft> m_Aircrafts;
	std::map<int, int> m_LastPings; // TBD - How do we handle Unknown aircraft?
	//std::vector<Environment> m_Environment;

	Airspace();
	virtual ~Airspace();
};
