#include "fdpch.h"
#include "Client.h"

namespace Foundation
{
	void Client::PingServer()
	{
		Net::Message<NetMessageTypes> message;
		message.m_Header.m_ID = NetMessageTypes::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		message << timeNow;

		Send(message);
	}

	void Client::MessageAll()
	{
		Net::Message<NetMessageTypes> message;
		message.m_Header.m_ID = NetMessageTypes::MessageAll;
		Send(message);
	}
}