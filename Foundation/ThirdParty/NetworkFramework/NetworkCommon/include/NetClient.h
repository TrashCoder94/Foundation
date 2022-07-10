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
#include "NetMessage.h"
#include "NetTSQueue.h"
#include "NetConnection.h"

namespace Net
{
	template<typename T>
	class Client
	{
		public:
			Client() :
				m_Socket(m_AsioContext)
			{
				// Initialise the socket with the io context, so it can do stuff
			}

			virtual ~Client()
			{
				// If the client is destroyed, always disconnect from the server.
				Disconnect();
			}

			// Connect to server with hostname/ip-address and port
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					// Resolve hostname/ip-address into tangiable physical address
					asio::ip::tcp::resolver resolver(m_AsioContext);
					asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
					
					// Create connection
					m_Connection = std::make_unique<Connection<T>>(
						Connection<T>::Owner::Client,
						m_AsioContext,
						asio::ip::tcp::socket(m_AsioContext),
						m_QueueMessagesIn);

					// Tell the connection object to connect to the server.
					m_Connection->ConnectToServer(endpoints);

					// Start thread context;
					m_ThreadContext = std::thread([this]() { m_AsioContext.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "[CLIENT] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[CLIENT] Connected!\n";
				return true;
			}

			// Disconnect from server
			void Disconnect()
			{
				if (IsConnected())
				{
					m_Connection->Disconnect();
				}

				// Either way, we're also done with the asio context.
				m_AsioContext.stop();

				// And it's thread.
				if (m_ThreadContext.joinable())
					m_ThreadContext.join();

				m_Connection.release();
			}

			// Check if client is actually connected to a server.
			bool IsConnected()
			{
				if (m_Connection)
					return m_Connection->IsConnected();
				else
					return false;
			}

			// Sends a message to the server.
			void Send(const Message<T>& message)
			{
				if (IsConnected())
					m_Connection->Send(message);
			}

			// Retrieve queue of messages from server.
			TSQueue<OwnedMessage<T>>& GetIncomingMessages()
			{
				return m_QueueMessagesIn;
			}

		protected:
			// Asio handles data transfer.
			asio::io_context m_AsioContext;

			// Needs a thread of it's own to execute its work commands
			std::thread m_ThreadContext;

			// This is the hardware socket that is connected to the server.
			asio::ip::tcp::socket m_Socket;

			// The client has a single instance of a "connection" object, which handles data transfer.
			std::unique_ptr<Connection<T>> m_Connection;

		private:
			// This is the thread safe queue of incoming messages from server.
			TSQueue<OwnedMessage<T>> m_QueueMessagesIn;
	};
}