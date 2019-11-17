#pragma once

#include "Logger.h"
#include "Airspace.h"
#include "Aircraft.h"

class Radar {
public:
	static Radar& getInstance();
	void CollisionPrediction(int period = 180);
	bool AdvanceTime();
	std::vector<Aircraft> Report();

private:
	Logger& m_Logger = Logger::getInstance();
	Airspace& m_Airspace = Airspace::getInstance();

	int m_Time = 0;
	int m_Scan_Interval = 15;
	std::vector<Aircraft> m_Aircrafts;

	void ProcessTime();

	Radar(int scan_interval = 15);
	virtual ~Radar();
};
