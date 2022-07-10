#pragma once

#include <NetCommon.h>
#include "MessageTypes.h"

namespace Foundation
{
	class Server : public Net::Server<NetMessageTypes>
	{
		public:
			Server(uint16_t port);

			// Called when a client connects, you can veto the connection by returning false.
			bool OnClientConnect(const std::shared_ptr<Net::Connection<NetMessageTypes>>& client) override;

			// Called when a client appears to have disconnected.
			void OnClientDisconnect(const std::shared_ptr<Net::Connection<NetMessageTypes>>& client) override;

			// Called when a message arrives.
			void OnMessage(std::shared_ptr<Net::Connection<NetMessageTypes>>& client, const Net::Message<NetMessageTypes>& message) override;
	};
}