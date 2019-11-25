#include "Communications.h"
#include <iostream> // FOR DEBUGGING

using namespace std;

Communications& Communications::getInstance() {
	static Communications _instance;
	return _instance;
}

Communications::Communications() {

	m_ProcessingThread = new thread{ &Communications::ProcessMessages, this };

	cout << "Communications instantiated" << endl;
}

void Communications::ProcessMessages() {
	while (m_SystemOnline)
	{
		// Wait to receive a message
		unique_lock<mutex> messagelock(m_MsgMutex);
		m_Cond_NotEmpty.wait(messagelock,
					[&a = m_SystemOnline, &mList = m_Messages]
					{ return (!a || !mList.empty()); });
					// To prevent spurious wakeup make sure there are messages to send

		// copy the message to a local var
		vector<Message> copy{ m_Messages };

		// clear the message list
		m_Messages.clear();

		// release lock
		messagelock.unlock();

		// send messages
		for (Message msg : copy)
		{
			if (msg.type >=1 && msg.type <= 5) { // Message is for a single aircraft
				airspace.SendMessage(msg);
			}
			else if (msg.type >= 6 && msg.type <= 8) { // Broadcast
				airspace.Broadcast(msg);
			}
		}
	}
}

void Communications::QueueMessage(Message msg) {
	if (m_SystemOnline) {
		lock_guard<mutex> msgmutex(m_MsgMutex);
		m_Messages.push_back(msg);
	}
	m_Cond_NotEmpty.notify_one();
}

vector<string> Communications::GetResponses() {
	return airspace.GetResponses();
}
