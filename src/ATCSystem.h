#pragma once

#include "Aircraft.h"
#include "Airspace.h"
#include "Radar.h"
#include "Communications.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

class ATC_System {

public:
	static ATC_System& getInstance();
	void print();
	void link_aircraft(const vector<Aircraft> &ac);
	void print_grid();



	void process_time();

	void show_plan(bool);

	void Resume();
	void Pause();

private:
	int m_Milliwait = 100;
	int m_Time = 0;
	bool m_SystemOnline = true;
	bool m_Paused = true;
	bool m_Show_Display = false;
	std::mutex m_TimeMutex; // For accessing time
	std::condition_variable m_Cond_Time;



	Airspace& airspace = Airspace::getInstance();
	Communications& comms = Communications::getInstance();
	Radar& radar = Radar::getInstance();

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
	vector<Aircraft> aircraft;	// pointer to vector of known aircraft

	// member functions
	char check_position(const int x, const int y) const;

	// constructors
	ATC_System();
	virtual ~ATC_System();
};
