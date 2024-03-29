#pragma once

#include "Radar.h"
#include "Aircraft.h"
#include "Airspace.h"
#include "Communications.h"
#include "Logger.h"
#include "Message.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <algorithm>

/*
// System parameters // I would love to declare them here but it creates cyclic dependencies
#define RADAR_INTERVAL 5
#define DISPLAY_INTERVAL 5
#define LOG_INTERVAL 15
#define PREDICTION_WINDOW 180

#define MIN_SEP_X 3*5280
#define MIN_SEP_Y 3*5280
#define MIN_SEP_Z 1000
*/

class ATC_System {

public:
	static ATC_System& getInstance();
	void print_grid();

	void process_time();

	void show_plan(bool);

	void Resume();
	void Pause();
	std::vector<Aircraft> getAircraft();

	void SendComms(Message);
	void showMessages();
	void clearMessages();
	void ReadLog();

private:
	int m_Milliwait = 1000;
	int m_Time = 0;
	bool m_SystemOnline = true;
	bool m_Paused = true;
	bool m_Show_Display = false;
	std::mutex m_TimeMutex; // For accessing time
	std::mutex m_AircraftMutex; // For accessing time
	std::mutex m_WarningMutex; // For accessing time
	std::mutex m_ResponseMutex; // For accessing time
	std::condition_variable m_Cond_Time;

	Airspace& airspace = Airspace::getInstance();
	Communications& comms = Communications::getInstance();
	Radar& radar = Radar::getInstance();
	Logger& m_Logger = Logger::getInstance();

	std::thread* m_ProcessingThread = nullptr;

	// member variables
	// environment properties
	int x_size = 100;	// airspace x-dim in miles
	int y_size = 100;	// airspace y-dim in miles
	int z_size = 25000;	// airspace z-dim in feet

	int min_x_sep = 3;			// minimum aircraft x separation in miles
	int min_y_sep = min_x_sep;	// minimum aircraft y separation in miles
	int min_z_sep = 1000;		// minimum aircraft z separation in feet
	// end environment properties
	int quadrant_size = 4;		// number of rows and columns per quadrant
	std::vector<Aircraft> m_Aircraft;	// vector of known aircraft
	std::vector<std::string> m_Warnings; // vector of most recent warnings
	std::vector<std::string> m_Responses; // vector of all pilot messages received

	// member functions
	char check_position(const int x, const int y) const;

	// constructors
	ATC_System();
};
