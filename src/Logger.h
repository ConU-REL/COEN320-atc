#pragma once
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Aircraft.h"

class Logger {
public:
	static Logger& getInstance();

	void Log(std::vector<Aircraft>);
	void Read();

private:
	Logger();

	bool m_SystemOnline = true;
	void ProcessEntries();

	std::thread* m_ProcessingThread = nullptr;
	std::vector<std::string> m_Data;
	std::mutex m_DataMutex; // For accessing the data queue
	std::mutex m_FileMutex; // For accessing the log file
	std::condition_variable m_Cond_NotEmpty;
};
