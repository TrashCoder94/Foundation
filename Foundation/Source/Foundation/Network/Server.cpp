#include "fdpch.h"
#include "Server.h"

namespace Foundation
{
	Server::Server(uint16_t port) : Net::Server<NetMessageTypes>(port)
	{}

	bool Server::OnClientConnect(const std::shared_ptr<Net::Connection<NetMessageTypes>>& client)
	{
		Net::Message<NetMessageTypes> message;
		message.m_Header.m_ID = NetMessageTypes::ServerAccept;
		client->Send(message);
		return true;
	}

	void Server::OnClientDisconnect(const std::shared_ptr<Net::Connection<NetMessageTypes>>& client)
	{
		FD_CORE_LOG_INFO("[SERVER] - Removing Client ['{0}']", client->GetID());
	}

	void Server::OnMessage(std::shared_ptr<Net::Connection<NetMessageTypes>>& client, const Net::Message<NetMessageTypes>& message)
	{
		switch (message.m_Header.m_ID)
		{
			case NetMessageTypes::ServerAccept:
			{
				break;
			}
			case NetMessageTypes::ServerDeny:
			{
				break;
			}
			case NetMessageTypes::ServerPing:
			{
				FD_CORE_LOG_INFO("[SERVER] - Client ['{0}'] Server Ping", client->GetID());
				client->Send(message);
				break;
			}
			case NetMessageTypes::ServerMessage:
			{
				break;
			}
			case NetMessageTypes::MessageAll:
			{
				FD_CORE_LOG_INFO("[SERVER] - Client ['{0}'] Message All", client->GetID());
				Net::Message<NetMessageTypes> message;
				message.m_Header.m_ID = NetMessageTypes::ServerMessage;
				message << client->GetID();
				MessageAllClients(message, client);
				break;
			}
		}
	}
}