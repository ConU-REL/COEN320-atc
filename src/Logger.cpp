#include "Logger.h"
#include <iostream>

using namespace std;

Logger& Logger::getInstance() {
	static Logger _instance;
	return _instance;
}

Logger::Logger() {
	cout << "Logger instantiated" << endl;
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

