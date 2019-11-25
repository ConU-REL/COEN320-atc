#pragma once


#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include "Aircraft.h"
#include "Message.h"

// To avoid cyclic dependency these parameter are listed here
#define RADAR_INTERVAL 10
#define DISPLAY_INTERVAL 5
#define LOG_INTERVAL 60
#define PREDICTION_WINDOW 180
#define HOLDING_TIMER 10 // duration for which a holding aircraft flies before turning to form a loop
//#define HOLDING_SPEED 50 // In aircraft.h to avoid cyclic dependency

#define MIN_SEP_X 3*5280
#define MIN_SEP_Y 3*5280
#define MIN_SEP_Z 1000

class Airspace {

public:
	static Airspace& getInstance();
	std::vector<Aircraft> Scan();

	bool AdvanceTime(int seconds = 1);
	void AddAircraft();
	bool ChangeAircraft(Aircraft&);
	std::vector<Aircraft> getActiveAircraft();
	std::vector<Aircraft> getAircraftDataSet();

	void SendMessage(Message);
	void Broadcast(Message);
	std::vector<std::string> GetResponses();

private:
	int next_unique_id = 0;

	int m_Time = 0;
	int m_Next_Event;
	std::vector<std::string> m_Responses;
	std::vector<Message> m_Broadcasts;
	std::vector<Message> m_Messages;
	std::vector<Aircraft> m_A_Dataset;
	std::vector<Aircraft> m_Aircrafts;
	//std::vector<Environment> m_Environment;

	std::thread* m_ProcessingThread = nullptr;

	void ProcessMovement();

	Airspace();

	bool m_SystemOnline = true;
	std::mutex m_TimeMutex; // For accessing time
	std::mutex m_DataMutex; // For accessing the aircraft data set
	std::mutex m_ActiveMutex; // For accessing the active aircraft
	std::mutex m_MsgMutex; // For accessing the message queue
	std::mutex m_BroadMutex; // For accessing the broadcast queue
	std::mutex m_ResponseMutex; // For accessing the broadcast queue
	std::condition_variable m_Cond_Time;
	std::condition_variable m_Cond_Event;
};
