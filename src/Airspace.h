#ifndef AIRSPACE_H_
#define AIRSPACE_H_

#include <vector>
#include "Aircraft.h"

class Airspace {

class Record {
	Aircraft aircraft;
	int last_ping;
};

public:

	int m_Time = 0;
	std::vector<Record> m_Records;
	//std::vector<Environment> m_Environment;

	Airspace();
	virtual ~Airspace();
};

#endif /* AIRSPACE_H_ */
