#include <iostream>
#include <vector>

#include "ATCSystem.h"
#include "Airspace.h"
#include "Communications.h"
#include "Message.h"

using namespace std;

int main() {
	ATC_System& sys = ATC_System::getInstance();
	Airspace& airspace = Airspace::getInstance();

	vector<Aircraft> AircraftsCopy; // Vector used for local operations on aircraft

	char input; // Store user input for evaluation
	const char resume = 'R'; // Resume the simulation
	const char pause = 'P'; // Pause the simulation
	const char back = 'B'; // Back out of current menu (for char menus)
	const int escape = -1; // Back out of current menu (for integer menus)
	const char exit = 'X'; // Exit the application as a whole

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

				cout << endl << "Simulation status: RUNNING" << endl << endl;

				cout << "Enter a command" << endl;
				cout << "\t" << pause << ")STOP - Pause Simulation" << endl << endl;

				cout << "\t1)SHOW - Show the display" << endl;
				cout << "\t2)SEND - View aircraft and send messages" << endl;
				cout << "\t3)MESG - View received messages" << endl;
				cout << "\t4)HIST - View the history log file" << endl;

				cout << endl << "\t" << exit << ")Exit application" << endl;
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
					char cmd = '\0';
					while (cmd != back) {
						AircraftsCopy = sys.getAircraft();
						for (Aircraft& ac : AircraftsCopy) {
							ac.PrintMembers();
						}
						cout << endl << "Simulation status: RUNNING" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << back << ")Back to previous menu" << endl << endl;

						cout << "\t1)Send a message" << endl;
						cout << "\t2)Broadcast a message" << endl;


						// Maybe put a confirmation for exit

						cmd = '\0';
						cin >> cmd;

						if (cmd == '1') { // Send a message to an identifies aircraft
							int aid = 0;
							while (aid != escape) {


								cout << "Enter a command" << endl;
								cout << "\t" << escape << ")Back to previous menu" << endl;
								cout << "\t-5) Display aircraft information" << endl << endl;

								cout << "\tEnter the ID of the Aircraft you wish to message" << endl;

								aid = '\0';
								cin >> aid;

								if (aid != -1) {
									if (cin.fail()) {
										cout << "INVALID ID FORMAT (Integers only please)" << endl;
										cin.clear();
										cin.ignore(10000, '\n');
									}
									else if (aid == -5) {
										AircraftsCopy = sys.getAircraft();
										for (Aircraft& ac : AircraftsCopy) {
											ac.PrintMembers();
										}
									}
									else{
										auto it = find_if(AircraftsCopy.begin(), AircraftsCopy.end(), [&aid](const Aircraft& ac) {return ac.a_id == aid;});

										if (it != AircraftsCopy.end())
										{
											char msg = '\0';
											while (msg != back) {
												cout << "Choose a message to send" << endl;
												cout << "\t" << back << ")Back to previous menu" << endl << endl;

												cout << "\t1)Report position" << endl;
												cout << "\t2)Enter holding pattern" << endl;
												cout << "\t3)Exit holding pattern" << endl;
												cout << "\t4)Change velocity" << endl;
												cout << "\t5)Change elevation" << endl;


												// Maybe put a confirmation for exit

												msg = '\0';
												cin >> msg;

												Message message;
												message.dest_id = it->a_id;

												if (msg == '1') { // Report position
													message.type = 1;
													sys.SendComms(message);
												}
												else if (msg == '2') { // Enter holding pattern
													message.type = 2;
													sys.SendComms(message);
												}
												else if (msg == '3') { // Exit holding pattern
													message.type = 3;
													sys.SendComms(message);
												}
												else if (msg == '4') { // Change velocity
													message.type = 4;
													Velocity v;
													cout << endl << "\tEnter X Velocity: ";
													cin >> v.vx;
													cout << endl << "\tEnter Y Velocity: ";
													cin >> v.vy;
													cout << endl << "\tEnter Z Velocity: ";
													cin >> v.vz;
													message.vel = v;
													sys.SendComms(message);
												}
												else if (msg == '5') { // Change elevation
													message.type = 5;
													cout << "Current elevation: " << it->cur_pos.pz << endl;
													cout << endl << "\tEnter Z Position: ";
													cin >> message.elevation;
													sys.SendComms(message);
												}
											}
										}
										else {
											cout << "No Aircraft with that ID was found" << endl;
										}
									}
								}
							}
						}
						else if (cmd == '2') { // Broadcast a message
							char msg = '\0';
							while (msg != back) {
								cout << "Choose a broadcast to send" << endl;
								cout << "\t" << back << ")Back to previous menu" << endl << endl;

								cout << "\t1)Report positions and velocities" << endl;
								cout << "\t2)Enter holding pattern" << endl;
								cout << "\t3)Exit holding pattern" << endl;


								// Maybe put a confirmation for exit

								msg = '\0';
								cin >> msg;

								Message message;

								if (msg == '1') { // Report position and velocity
									message.type = 6;
									sys.SendComms(message);
								}
								else if (msg == '2') { // Enter holding pattern
									message.type = 7;
									sys.SendComms(message);
								}
								else if (msg == '3') { // Exit holding pattern
									message.type = 8;
									sys.SendComms(message);
								}
							}
						}
					}
				}
				else if (input == '3') { // View received messages
					char cmd = '\0';
					while (cmd != back) {
						sys.showMessages();
						cout << endl << "Simulation status: RUNNING" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << back << ")Back to previous menu" << endl << endl;

						cout << "\t1)Delete messages" << endl;

						cmd = '\0';
						cin >> cmd;

						if (cmd == '1') {
							sys.clearMessages();
						}
					}
				}
				else if (input == '4') { // View aircraft history log
					sys.ReadLog();
					char cmd = '\0';
					while (cmd != back) {
						cout << endl << "Simulation status: RUNNING" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << back << ")Back to previous menu" << endl << endl;

						cmd = '\0';
						cin >> cmd;
					}
				}
			} // while (input != exit && input != pause)
			sys.Pause();
		}
		else if (input == '1') { // List information for ALL aircraft
			while (input != back) {
				AircraftsCopy = airspace.getActiveAircraft();
				vector<Aircraft> tempCopy = airspace.getAircraftDataSet();
				AircraftsCopy.insert(AircraftsCopy.end(), tempCopy.begin(), tempCopy.end());

				cout << endl << "Simulation status: PAUSED" << endl << endl;

				cout << "Enter a command" << endl;
				cout << "\t" << back << ")Back to previous menu" << endl << endl;

				cout << "\t1)View/Edit aircraft data" << endl;
				cout << "\t2)Add an aircraft" << endl;
				cout << "\t3)Project aircraft positions" << endl;

				input = '\0';
				cin >> input;

				if (input == '1') { // List information for active aircraft and prompt user
					int sid = '\0'; // System ID for the aircraft
					while (sid != escape) {
						for (Aircraft& ac : AircraftsCopy) {
							ac.PrintFullMembers();
						}

						cout << endl << "Simulation status: PAUSED" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << escape << ")Back to previous menu" << endl << endl;

						cout << "\tEnter the SID of the Aircraft you wish to modify" << endl;

						sid = '\0';
						cin >> sid;

						if (sid != -1) {
							if (cin.fail()) {
								cout << "INVALID SID FORMAT (Integers only please)" << endl;
								cin.clear();
								cin.ignore(10000, '\n');
							}
							else{
								auto it = find_if(AircraftsCopy.begin(), AircraftsCopy.end(), [&sid](const Aircraft& ac) {return ac.s_id == sid;});

								if (it != AircraftsCopy.end())
								{
								  // found element. 'it' is an iterator to the first matching element
								  if (airspace.ChangeAircraft(*it)) {
									  AircraftsCopy.erase(it);
								  }

								  // if you really need the index, you can also get it:
								  // auto index = std::distance(localAcCopy.begin(), it);
								  //airspace.ChangeAircraft(localAcCopy[index]);
								}
								else {
									cout << "No Aircraft with that SID was found" << endl;
								}
							}
						}
					}
				}
				else if (input == '2') { // Add aircraft
					airspace.AddAircraft();
				}
				else if (input == '3') { // Project aircraft positions
					for (Aircraft& ac : AircraftsCopy) {
						ac.PrintFullMembers();
					}
					int seconds = 0;
					while (seconds != escape) {
						cout << endl << "Simulation status: PAUSED" << endl << endl;

						cout << "Enter a command" << endl;
						cout << "\t" << escape << ")Back to previous menu" << endl << endl;

						cout << "Enter a time (in seconds) for which to perform position projection" << endl;

						seconds = 0;
						cin >> seconds;

						if (seconds != -1) {
							if (cin.fail()) {
								cout << "INVALID TIME FORMAT (Integers only please)" << endl;
								cin.clear();
								cin.ignore(10000, '\n');
							}
							else{
								vector<Aircraft> projection = AircraftsCopy;
								for (Aircraft& ac : projection) {
									ac.fly(seconds);
									ac.PrintFullMembers();
								}
							}
						}
					}

				}
			}
		}
	}

	cout << "Simulation terminated." << endl;
	return 0;
}
