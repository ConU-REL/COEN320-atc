#include <iostream>
#include "Airspace.h"
#include "Communications.h"
#include "Radar.h"
#include <vector>
#include "ATCSystem.h"

using namespace std;

bool frozen;

int main() {
	int time = 0;
	int milliwait = 50;
	frozen = true;

	ATC_System& sys = ATC_System::getInstance();
	vector<Aircraft> aircrafts;

	char input;
	char exit = 'X'; // Exit the application as a whole
	char resume = 'R';
	char pause = 'P'; // Pause the simulation

	while (input != exit) { // Pause menu
		cout << endl << "Simulation status: PAUSED" << endl << endl;

		cout << "Enter a command" << endl;
		cout << "\t" << resume << ")Resume simulation" << endl;
		cout << "\t2)View/Edit Aircraft data" << endl;

		cout << "\t" << exit << ")Exit application" << endl;
		// Maybe put a confirmation for exit

		input = '\0';
		cin >> input;

		if (input == resume) { // Resume simulation time and go to active menu
			sys.Resume();
			while (input != exit && input != pause) {

				cout << "Simulation status: RUNNING" << endl << endl;

				cout << "Enter a command" << endl;
				cout << "\t1)SHOW - Plan View and Message Display" << endl;
				cout << "\t2)DATA - View active aircraft information" << endl;
				cout << "\t" << pause << ")STOP - Pause Simulation" << endl;

				cout << "\tX)Exit application" << endl;
				// Maybe put a confirmation for exit

				input = '\0';
				cin >> input;

				if (input == '1') { // Show the display
					sys.show_plan(true);
					bool cont = true;
					while (cont) { // Continue to show the plan until input received
						input = '\0';
						cin >> input; // Annoyingly we need to enter some kind of character, just Enter/Return is not enough
						input = '\0';
						sys.show_plan(false);
						cont = false;
					}
				}
				else if (input == '2') { // List information for active aircraft

				}
			}
			sys.Pause();
		}
	}



	while (!frozen) {
		time++;
		//airspace.AdvanceTime();
		//radar.AdvanceTime();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Delay for radar to be able to read from airspace
		if (time % RADAR_INTERVAL == 0) {
			//aircrafts = radar.Report();
		}

		if (time % DISPLAY_INTERVAL == 0) {
			sys.link_aircraft(aircrafts);
			sys.print_grid();
			for (Aircraft ac : aircrafts) {
				ac.PrintMembers();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(milliwait));
	}

	cout << "Simulation terminated." << endl;
	return 0;
}
