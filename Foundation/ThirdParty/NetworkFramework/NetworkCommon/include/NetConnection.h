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

namespace Net
{
	template<typename T>
	class Server;

	template<typename T>
	class Connection : public std::enable_shared_from_this<Connection<T>>
	{
		public:
			enum class Owner
			{
				Server,
				Client
			};

			Connection(Owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, TSQueue<OwnedMessage<T>>& queueIncomingMessages) :
				m_OwnerType(parent),
				m_AsioContext(asioContext),
				m_Socket(std::move(socket)),
				m_QueueMessagesIn(queueIncomingMessages),
				m_QueueMessagesOut(),
				m_ID(0),
				m_HandshakeOut(0),
				m_HandshakeIn(0),
				m_HandshakeCheck(0)
			{
				// Construct validation check data.
				if (m_OwnerType == Owner::Server)
				{
					// Connection is Server -> Client, construct random data for the client
					// to transform and send back for validation
					m_HandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

					// Pre-calculate the result for checking when the client responses
					m_HandshakeCheck = Scramble(m_HandshakeOut);
				}
				else
				{
					// Connection is Client -> Server, so we have nothing to define.
					m_HandshakeIn = 0;
					m_HandshakeOut = 0;
				}
			}

			virtual ~Connection()
			{}

			uint32_t GetID() const
			{
				return m_ID;
			}

		public:
			void ConnectToClient(Net::Server<T>* pServer, uint32_t uid = 0)
			{
				// Only relevant to servers.
				if (m_OwnerType == Owner::Server)
				{
					if (m_Socket.is_open())
					{
						m_ID = uid;
						//ReadHeader();

						// A client has attempted to connect to the server, but we wish
						// the client to first validate itself, so first write out the 
						// handshake data to be validated.
						WriteValidation();

						// Next, issue a task to sit and wait asynchronously for
						// the validation data to be sent back from the client.
						ReadValidation(pServer);
					}
				}
			}

			void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
			{
				// Only relevant to clients.
				if (m_OwnerType == Owner::Client)
				{
					asio::async_connect(m_Socket, endpoints, 
						[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
						{
							if (!ec)
							{
								// ReadHeader();

								// First thing server will do is send packet to be validated
								// so wait for that and respond
								ReadValidation();
							}
						});
				}
			}

			void Disconnect()
			{
				if (IsConnected())
					asio::post(m_AsioContext, [this]() { m_Socket.close(); });
			}

			bool IsConnected() const
			{
				return m_Socket.is_open();
			}

		public:
			void Send(const Message<T>& message)
			{
				asio::post(m_AsioContext, 
					[this, message]()
					{
						bool writingMessage = !m_QueueMessagesOut.empty();
						m_QueueMessagesOut.push_back(message);

						if (!writingMessage)
						{
							WriteHeader();
						}
					});
			}

		private:
			// ASYNC - Prime context ready to read a message header
			void ReadHeader()
			{
				asio::async_read(m_Socket, asio::buffer(&m_TempMessage.m_Header, sizeof(MessageHeader<T>)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (m_TempMessage.m_Header.m_Size > 0)
							{
								m_TempMessage.m_Body.resize(m_TempMessage.m_Header.m_Size);
								ReadBody();
							}
							else
							{
								AddToIncomingMessageQueue();
							}
						}
						else
						{
							std::cout << "[" << m_ID << "] Read Header Fail.\n";
							m_Socket.close();
						}
					});
			}

			// ASYNC - Prime context ready to read a message body
			void ReadBody()
			{
				asio::async_read(m_Socket, asio::buffer(m_TempMessage.m_Body.data(), m_TempMessage.m_Body.size()),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							AddToIncomingMessageQueue();
						}
						else
						{
							std::cout << "[" << m_ID << "] Read Body Fail.\n";
							m_Socket.close();
						}
					});
			}

			// ASYNC - Prime context ready to write a message header
			void WriteHeader()
			{
				asio::async_write(m_Socket, asio::buffer(&m_QueueMessagesOut.front().m_Header, sizeof(MessageHeader<T>)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (m_QueueMessagesOut.front().m_Body.size() > 0)
							{
								WriteBody();
							}
							else
							{
								m_QueueMessagesOut.pop_front();

								if(!m_QueueMessagesOut.empty())
								{
									WriteHeader();
								}
							}
						}
						else
						{
							std::cout << "[" << m_ID << "] Write Header Fail.\n";
							m_Socket.close();
						}
					});
			}

			// ASYNC - Prime context ready to write a message body
			void WriteBody()
			{
				asio::async_write(m_Socket, asio::buffer(m_QueueMessagesOut.front().m_Body.data(), m_QueueMessagesOut.front().m_Body.size()),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							m_QueueMessagesOut.pop_front();

							if (!m_QueueMessagesOut.empty())
							{
								WriteHeader();
							}
						}
						else
						{
							std::cout << "[" << m_ID << "] Write Body Fail.\n";
							m_Socket.close();
						}
					});
			}

			void AddToIncomingMessageQueue()
			{
				if (m_OwnerType == Owner::Server)
					m_QueueMessagesIn.push_back({ this->shared_from_this(), m_TempMessage });
				else
					m_QueueMessagesIn.push_back({ nullptr, m_TempMessage });

				ReadHeader();
			}

			uint64_t Scramble(uint64_t input)
			{
				uint64_t out = input ^ 0xD3ADB33FC0D3CAF3;
				out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
				return out ^ 0xC0D3FAC312345678;
				//return out ^ 0xKILOMINXISPRETTY;
			}

			void WriteValidation()
			{
				asio::async_write(m_Socket, asio::buffer(&m_HandshakeOut, sizeof(uint64_t)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// Validation data sent, clients should sit and wait
							// for a response.
							if (m_OwnerType == Owner::Client)
							{
								ReadHeader();
							}
						}
						else
						{
							m_Socket.close();
						}
					}
				);
			}

			void ReadValidation(Net::Server<T>* pServer = nullptr)
			{
				asio::async_read(m_Socket, asio::buffer(&m_HandshakeIn, sizeof(uint64_t)),
					[this, pServer](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (m_OwnerType == Owner::Server)
							{
								if (m_HandshakeIn == m_HandshakeCheck)
								{
									// Client has provided a valid solution, so allow it to connect.
									std::cout << "Client Validated" << std::endl;
									pServer->OnClientValidated(this->shared_from_this());

									// Sti waiting to receive data now.
									ReadHeader();
								}
								else
								{
									// Client gave incorrect data, close the connection.
									std::cout << "Client Disconnected (Fail Validation)" << std::endl;
									m_Socket.close();
								}
							}
							else
							{
								m_HandshakeOut = Scramble(m_HandshakeIn);

								WriteValidation();
							}
						}
						else
						{
							std::cout << "Client Disconnected (ReadValidation)" << std::endl;
							m_Socket.close();
						}
					}
				);
			}

		protected:
			Owner m_OwnerType;

			// Each connection has a unique socket to a remote
			asio::ip::tcp::socket m_Socket;

			// This context is shared with the whole asio instance
			asio::io_context& m_AsioContext;

			// This queue holds all messages that have been received from
			// the remote side of this connection. Note it is a reference
			// as the "owner"  of this connection is expected to provide a queue.
			TSQueue<OwnedMessage<T>>& m_QueueMessagesIn;

			// This queue holds all messages to be sent to the remote side 
			// of this connection
			TSQueue<Message<T>> m_QueueMessagesOut;

			Message<T> m_TempMessage;

			uint32_t m_ID;

			uint64_t m_HandshakeOut;
			uint64_t m_HandshakeIn;
			uint64_t m_HandshakeCheck;
	};
}