#pragma once

//#include "ATCSystem.h" // creates cyclic dependency
#include "Airspace.h"
#include "Aircraft.h"

#include <mutex>
#include <condition_variable>
#include <string>

class Radar {
public:
	static Radar& getInstance();
	void CollisionPrediction(int period = 180);
	bool AdvanceTime(int seconds = 1);
	std::vector<Aircraft> Report();
	std::vector<std::string> Warnings();

private:

	Airspace& m_Airspace = Airspace::getInstance();

	int m_Time = 0;
	int m_Scan_Interval = 15;
	std::vector<Aircraft> m_Aircrafts;
	std::vector<std::string> m_Warnings;

	std::thread* m_ProcessingThread = nullptr;
	void ProcessTime();
	void PredictSeparationViolation(const Aircraft, const Aircraft, int time = PREDICTION_WINDOW);

	Radar(int scan_interval = 15);

	bool m_SystemOnline = true;
	std::mutex m_TimeMutex; // For accessing time
	std::mutex m_AircraftMutex; // For accessing the scanned aircraft set
	std::mutex m_WarningMutex; // For accessing the warnings
	std::condition_variable m_Cond_ScanTime;
	std::condition_variable m_Cond_ReportReady;
};
