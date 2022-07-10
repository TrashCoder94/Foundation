/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9

	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

*/

#pragma once

#include "Common.h"
#include "NetTSQueue.h"
#include "NetMessage.h"
#include "NetConnection.h"

namespace Net
{
	template<typename T>
	class Server
	{
		public:
			Server(uint16_t port) :
				m_AsioAcceptor(m_AsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~Server()
			{
				Stop();
			}

			bool Start()
			{
				try
				{
					WaitForClientConnection();
					m_ThreadContext = std::thread([this]() { m_AsioContext.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "[SERVER] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[SERVER] Started!\n";
				return true;
			}

			void Stop()
			{
				m_AsioContext.stop();

				if (m_ThreadContext.joinable())
					m_ThreadContext.join();

				std::cout << "[SERVER] Stopped!\n";
			}

			// ASYNC - Instruct asio to wait for connection
			void WaitForClientConnection()
			{
				m_AsioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;
						
						std::shared_ptr<Connection<T>> newConnection = std::make_shared<Connection<T>>(
							Connection<T>::Owner::Server,
							m_AsioContext, std::move(socket), m_QueueMessagesIn);

						// Give the user server a chance to deny the connection.
						if (OnClientConnect(newConnection))
						{
							// Connection allowed, so add to container of new connections.
							m_DequeConnections.push_back(std::move(newConnection));

							m_DequeConnections.back()->ConnectToClient(this, m_IDCounter++);

							std::cout << "[" << m_DequeConnections.back()->GetID() << "] Connection Approved\n";
						}
						else
						{
							std::cout << "[SERVER] Connection Denied\n";
						}
					}
					else
					{
						std::cout << "[SERVER] New Connection Error: " << ec.message() << std::endl;
					}

					// Prime the asio context with more work - again simply wait for 
					// another connection...
					WaitForClientConnection();
				});
			}

			void MessageClient(std::shared_ptr<Connection<T>>& client, const Message<T>& message)
			{
				if (client && client->IsConnected())
				{
					client->Send(message);
				}
				else
				{
					OnClientDisconnect(client);
					client.reset();
					m_DequeConnections.erase(
						std::remove(m_DequeConnections.begin(), m_DequeConnections.end(), client), m_DequeConnections.end());
				}
			}
			
			void MessageAllClients(const Message<T>& message, std::shared_ptr<Connection<T>>& pIgnoreClient = nullptr)
			{
				bool invalidClient = false;
				for (auto& client : m_DequeConnections)
				{
					if (client && client->IsConnected())
					{
						if (client != pIgnoreClient)
							client->Send(message);
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						invalidClient = true;
					}
				}

				if (invalidClient)
				{
					m_DequeConnections.erase(
						std::remove(m_DequeConnections.begin(), m_DequeConnections.end(), nullptr), m_DequeConnections.end());
				}
			}

			void Update(size_t maxMessages = -1, bool bWait = false)
			{
				if (bWait)
					m_QueueMessagesIn.wait();

				size_t messageCount = 0;
				while (messageCount < maxMessages && !m_QueueMessagesIn.empty())
				{
					auto message = m_QueueMessagesIn.pop_front();

					// Pass to message handler
					OnMessage(message.m_Remote, message.m_Message);

					messageCount++;
				}
			}

		protected:
			// Called when a client connects, you can veto the connection by returning false.
			virtual bool OnClientConnect(const std::shared_ptr<Connection<T>>& client)
			{
				return false;
			}

			// Called when a client appears to have disconnected.
			virtual void OnClientDisconnect(const std::shared_ptr<Connection<T>>& client)
			{

			}

			// Called when a message arrives.
			virtual void OnMessage(std::shared_ptr<Connection<T>>& client, const Message<T>& message)
			{

			}

		public:
			// Called when a client is validated
			virtual void OnClientValidated(std::shared_ptr<Connection<T>> client)
			{

			}

		protected:
			// Thread safe queue for incoming message packets.
			TSQueue<OwnedMessage<T>> m_QueueMessagesIn;

			// Container of active connections.
			std::deque<std::shared_ptr<Connection<T>>> m_DequeConnections;

			asio::io_context m_AsioContext;
			std::thread m_ThreadContext;

			// These need an asio context.
			asio::ip::tcp::acceptor m_AsioAcceptor;

			// Clients will be identified in the "wider system" via IDs.
			uint32_t m_IDCounter = 10000;
	};
}