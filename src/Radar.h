#pragma once

#include "Logger.h"

class Radar {
public:
	static Radar& getInstance();

	Logger& logger = Logger::getInstance();

	Radar();
	virtual ~Radar();
};
