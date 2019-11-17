#pragma once


#include <vector>
#include <iostream>
#include "Aircraft.h"

class Record {
	Aircraft aircraft;
	int last_ping;
};


class Airspace {

public:
	static Airspace& getInstance();
	void CollisionPrediction(int period = 180);

private:
	int m_Time = 0;
	std::vector<Record> m_Records;
	//std::vector<Environment> m_Environment;

	Airspace();
	virtual ~Airspace();
};
