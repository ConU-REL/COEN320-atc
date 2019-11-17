#pragma once

class Logger {
public:
	static Logger& getInstance();

	Logger();
	virtual ~Logger();
};
