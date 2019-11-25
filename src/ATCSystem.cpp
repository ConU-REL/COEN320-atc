#include "ATCSystem.h"

#include <iostream>
#include <math.h>

using namespace std;


ATC_System& ATC_System::getInstance() {
	static ATC_System _instance;
	return _instance;
}

ATC_System::ATC_System() {
	m_ProcessingThread = new thread{ &ATC_System::process_time, this };
	cout << "Display instantiated" << endl;
}


void ATC_System::print_grid(){
	int grid_size_x = ceil((float)(x_size / min_x_sep) / quadrant_size) * quadrant_size;
	int grid_size_y = ceil((float)(y_size / min_y_sep) / quadrant_size) * quadrant_size;
	int elevation_chars = ceil((float)z_size/min_z_sep);
	int columns = grid_size_x/quadrant_size;

	// DEBUG
//	cout << "Grid Dimensions: " << grid_size_x << " x " << grid_size_y << endl;
//	cout << elevation_chars << " characters required to show Elevation Separation\n";

	int row = 1;
	char col = 65;
	for(int i=0; i < grid_size_x; i++){
		if(!(i % quadrant_size)){
			cout << " ";
		}
		if((float)(i % quadrant_size) == 2){
			cout << col++;
		} else {
			cout << " ";
		}

	}
	cout << "\n";
	for(int i=0; i < grid_size_x; i++){
		if(!(i % quadrant_size)){
			cout << string(grid_size_x+columns+1, '-') << endl;
		}
		for(int j=0; j < grid_size_y; j++){
			if(!(j % quadrant_size)){
				cout << "|";
			}

			cout << check_position(j, i);

		}
		cout << "|";
		if((float)(i % quadrant_size) == 1){
			cout << row++;
		}
		cout << "\n";
	}
	cout << string(grid_size_x+columns+1, '-') << endl;
}

char ATC_System::check_position(const int x, const int y) const{
	for(auto& ac : m_Aircraft){
		if(ac.grid_pos.px == x && ac.grid_pos.py == y){
			return 90 - ceil((float)ac.grid_pos.pz/min_z_sep);
		}
	}
	return 0;
}

void ATC_System::process_time() {
	while (m_SystemOnline)
	{
		// Wait for a unit of time to pass
		unique_lock<mutex> timelock(m_TimeMutex);
		m_Cond_Time.wait(timelock,
					[&paused = m_Paused]
					 { return (paused == false); });
		/*m_Cond_Time.wait(timelock,
			[&current_time = m_Time, &lt = last_time]
			 { return (current_time > lt); });*/
			// To prevent spurious wakeup make sure time has actually passed

		while (!m_Paused) {
			m_Time++;
			// Get current timestamp
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();


			airspace.AdvanceTime();
			radar.AdvanceTime();

			if (m_Time % RADAR_INTERVAL == 0) {
				lock_guard<mutex> aircraftlock(m_AircraftMutex);
				m_Aircraft = radar.Report();
				lock_guard<mutex> warninglock(m_WarningMutex);
				m_Warnings.clear();
				m_Warnings = radar.Warnings();
			}

			if (m_Show_Display && m_Time % DISPLAY_INTERVAL == 0) {
				if (m_Time % RADAR_INTERVAL != 0) { // Locally predict aircraft movement between radar scans
					for (Aircraft& ac : m_Aircraft) {
						ac.fly(DISPLAY_INTERVAL);
					}
				}
				else { // Don't reprint outdated radar warnings
					for (string& s : m_Warnings) {
						cout << s;
					}
				}
				print_grid();
			}

			{
				lock_guard<mutex> resplock(m_ResponseMutex);
				vector<string> tempCopy = comms.GetResponses();
				m_Responses.insert(m_Responses.end(), tempCopy.begin(), tempCopy.end());
			}

			if (m_Time % LOG_INTERVAL == 0) {
				lock_guard<mutex> aircraftlock(m_AircraftMutex);
				m_Logger.Log(m_Aircraft);
			}

			// Wait for difference between 1000 milliseconds and elapsed time since start of loop
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			auto int_ms = m_Milliwait - std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::this_thread::sleep_for(std::chrono::milliseconds(max((int)int_ms,1)));
			//cout << "Running..." << endl; // FOR DEBUGGING
		}
	}
}

void ATC_System::show_plan(bool mode) {
	print_grid();
	for (string& s : m_Warnings) {
		cout << s;
	}
	m_Show_Display = mode;
}

void ATC_System::Resume() {
	m_Paused = false;
	m_Cond_Time.notify_one();
}

void ATC_System::Pause() {
	m_Paused = true;
}

vector<Aircraft> ATC_System::getAircraft() {
	lock_guard<mutex> aircraftlock(m_AircraftMutex);
	return m_Aircraft;
}

void ATC_System::SendComms(Message message) {
	comms.QueueMessage(message);
}

void ATC_System::showMessages() {
	lock_guard<mutex> resplock(m_ResponseMutex);
	if (m_Responses.empty()) {
		cout << "No messages to display." << endl;
	}
	else {
		for (string msg : m_Responses) {
			cout << msg;
		}
	}
}

void ATC_System::clearMessages() {
	lock_guard<mutex> resplock(m_ResponseMutex);
	m_Responses.clear();
}

void ATC_System::ReadLog() {
	m_Logger.Read();
}
