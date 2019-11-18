#pragma once


#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Aircraft.h"

// For now I've put these definitions here but there should perhaps be a better place
#define RADAR_INTERVAL 10
#define DISPLAY_INTERVAL 5
#define LOG_INTERVAL 5

class Airspace {

public:
	static Airspace& getInstance();
	std::vector<Aircraft> Scan();

	bool AdvanceTime(int seconds = 1);
	void ChangeAircraft();

private:
	int m_Time = 0;
	int m_Next_Event;
	std::vector<Aircraft> m_A_Dataset;
	std::vector<Aircraft> m_Aircrafts;
	//std::vector<Environment> m_Environment;

	std::thread* m_ProcessingThread = nullptr;

	void ProcessMovement();

	Airspace();
	virtual ~Airspace();

	bool m_SimulationRunning = true;
	std::mutex m_TimeMutex; // For accessing time
	std::mutex m_DataMutex; // For accessing the aircraft data set
	std::mutex m_ActiveMutex; // For accessing the active aircraft
	std::condition_variable m_Cond_Time;
	std::condition_variable m_Cond_Event;
};
