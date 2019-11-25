#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Message.h"
#include "Airspace.h"

struct Communications {
public:
	static Communications& getInstance();

	void QueueMessage(Message);
	std::vector<std::string> GetResponses();

private:
	Airspace& airspace = Airspace::getInstance();

	bool m_SystemOnline = true;

	Communications();
	std::thread* m_ProcessingThread = nullptr;
	void ProcessMessages();

	std::vector<Message> m_Messages;

	std::mutex m_MsgMutex; // For accessing the message queue
	std::condition_variable m_Cond_NotEmpty;
};
