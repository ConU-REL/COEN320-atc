#include "Communications.h"
#include <iostream> // FOR DEBUGGING

using namespace std;

Communications& Communications::getInstance() {
	static Communications _instance;
	return _instance;
}

Communications::Communications() {
	cout << "Communications instantiated" << endl;

}

Communications::~Communications() {
	// TODO Auto-generated destructor stub
}

