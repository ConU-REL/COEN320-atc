#include "Logger.h"

using namespace std;

Logger& Logger::getInstance() {
	static Logger _instance;
	return _instance;
}

Logger::Logger() {
	m_ProcessingThread = new thread{ &Logger::ProcessEntries, this };
	remove("ATC_Log.txt");
	cout << "Logger instantiated" << endl;
}

void Logger::ProcessEntries() {
	while (m_SystemOnline)
	{
		// Wait to receive a message
		unique_lock<mutex> datalock(m_DataMutex);
		m_Cond_NotEmpty.wait(datalock,
					[&a = m_SystemOnline, &mList = m_Data]
					{ return (!a || !mList.empty()); });
					// To prevent spurious wakeup make sure there is data to log

		// copy the data to a local var
		vector<string> copy{ m_Data };

		// clear the data
		m_Data.clear();

		// release lock
		datalock.unlock();

		// log data
		lock_guard<mutex> filelock(m_FileMutex);

		ofstream myfile ("ATC_Log.txt", ios::app);
		if (myfile.is_open())
		{
			myfile << "================================================================================" << endl;
			std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
			std::time_t t = std::chrono::system_clock::to_time_t(p);
			myfile << ctime(&t);
			myfile << "--------------------------------------" << endl;
			for (string msg : copy)
			{
				myfile << msg;

			}
			myfile.close();
		}
		else {
			cout << "Unable to open log file";
		}
	}
}

void Logger::Log(vector<Aircraft> aircraftsToLog) {
	lock_guard<mutex> datalock(m_DataMutex);
	for (Aircraft& ac : aircraftsToLog) {
		m_Data.push_back(ac.Response());
	}
	m_Cond_NotEmpty.notify_one();
}

void Logger::Read() {
	lock_guard<mutex> filelock(m_FileMutex);
	string line;
	ifstream myfile ("ATC_Log.txt");
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
		  cout << line << '\n';
		}
		myfile.close();
		cout << "================================================================================" << endl;
	}
	else {
		cout << "Unable to open log file";
	}
}

