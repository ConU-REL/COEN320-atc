#include <iostream>
#include "Airspace.h"
#include "Communications.h"
#include "Radar.h"
#include "Display.h"
#include <vector>

using namespace std;

bool frozen;

int main() {
	int time = 0;
	int milliwait = 50;
	frozen = true;

	Airspace& airspace = Airspace::getInstance();
	Communications& comms = Communications::getInstance();
	Radar& radar = Radar::getInstance();
	Display& disp = Display::getInstance();


	vector<Aircraft> aircrafts;



	std::this_thread::sleep_for(std::chrono::milliseconds(milliwait));
	while (!frozen) {
		time++;
		airspace.AdvanceTime();
		radar.AdvanceTime();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Delay for radar to be able to read from airspace
		if (time % RADAR_INTERVAL == 0) {
			aircrafts = radar.Report();
		}

		if (time % DISPLAY_INTERVAL == 0) {
			disp.link_aircraft(aircrafts);
			disp.print_grid();
			for (Aircraft ac : aircrafts) {
				ac.PrintMembers();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(milliwait));
	}

	return 0;
}
