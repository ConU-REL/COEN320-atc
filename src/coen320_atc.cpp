#include <iostream>
#include "Airspace.h"
#include "Communications.h"
#include "Radar.h"
#include <vector>
#include "ATCSystem.h"

using namespace std;



int main() {
	ATC_System& sys = ATC_System::getInstance();
	Airspace& airspace = Airspace::getInstance();

	char input; // Store user input for evaluation
	char resume = 'R'; // Resume the simulation
	char pause = 'P'; // Pause the simulation
	char back = 'B';
	char exit = 'X'; // Exit the application as a whole

	while (input != exit) { // Pause menu
		cout << endl << "Simulation status: PAUSED" << endl << endl;

		cout << "Enter a command" << endl;
		cout << "\t" << resume << ")Resume simulation" << endl << endl;

		cout << "\t1)View/Edit Aircraft data" << endl;

		cout << endl << "\t" << exit << ")Exit application" << endl;
		// Maybe put a confirmation for exit

		input = '\0';
		cin >> input;

		if (input == resume) { // Resume simulation time and go to active menu
			sys.Resume();
			while (input != exit && input != pause) {

				cout << "Simulation status: RUNNING" << endl << endl;

				cout << "Enter a command" << endl;
				cout << "\t" << pause << ")STOP - Pause Simulation" << endl << endl;

				cout << "\t1)SHOW - Plan View and Message Display" << endl;
				cout << "\t2)DATA - View active aircraft information" << endl;

				cout << endl << "\tX)Exit application" << endl;
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
		else if (input == '1') { // List information for ALL aircraft
			while (input != back) {
				cout << endl << "Simulation status: PAUSED" << endl << endl;

				cout << "Enter a command" << endl;
				cout << "\t1)View/Edit Active aircraft data" << endl;
				cout << "\t2)View/Edit Future aircraft data" << endl;
				cout << "\t3)Add an aircraft" << endl;

				cout << endl << "\t" << back << ")Back to previous menu" << endl;

				input = '\0';
				cin >> input;

				if (input == '1') { // Show the display
					airspace.displayActiveAircraft();

					int sid; // System ID for the aircraft
					int escape = -1;
					while (sid != escape) {
						cout << endl << "Simulation status: PAUSED" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << escape << ")Back to previous menu" << endl << endl;
						cout << "\tEnter the SID of the Aircraft you wish to modify" << endl;

						sid = '\0';
						cin >> sid;

						if (sid != -1) {
							if (cin.fail()) {
								cout << "INVALID SID" << endl;
								cin.clear();
								cin.ignore(10000, '\n');
							}
							else{
								airspace.ChangeAircraft(sid);
							}
						}

					}

				}
				else if (input == '2') { // List information for active aircraft
					airspace.displayAircraftDataSet();
				}
			}
		}
	}

	cout << "Simulation terminated." << endl;
	return 0;
}
