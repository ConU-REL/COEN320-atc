#pragma once

#include "Logger.h"
#include "Airspace.h"
#include "Aircraft.h"

#include <mutex>
#include <condition_variable>

class Radar {
public:
	static Radar& getInstance();
	void CollisionPrediction(int period = 180);
	bool AdvanceTime(int seconds = 1);
	std::vector<Aircraft> Report();

private:
	Logger& m_Logger = Logger::getInstance();
	Airspace& m_Airspace = Airspace::getInstance();

	int m_Time = 0;
	int m_Scan_Interval = 15;
	std::vector<Aircraft> m_Aircrafts;

	std::thread* m_ProcessingThread = nullptr;
	void ProcessTime();

	Radar(int scan_interval = 15);
	virtual ~Radar();

	bool m_SystemOnline = true;
	std::mutex m_TimeMutex; // For accessing time
	std::mutex m_AircraftMutex; // For accessing the scanned aircraft set
	std::condition_variable m_Cond_ScanTime;
	std::condition_variable m_Cond_ReportReady;
};
