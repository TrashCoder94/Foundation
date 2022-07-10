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

#include <iostream>
#include <NetCommon.h>

// 8 bytes
enum class CustomMessageTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage
};

class CustomClient : public Net::Client<CustomMessageTypes>
{
	public:
		void PingServer()
		{
			Net::Message<CustomMessageTypes> message;
			message.m_Header.m_ID = CustomMessageTypes::ServerPing;

			std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
			message << timeNow;

			Send(message);
		}

		void MessageAll()
		{
			Net::Message<CustomMessageTypes> message;
			message.m_Header.m_ID = CustomMessageTypes::MessageAll;
			Send(message);
		}
};

int main()
{
	CustomClient client;
	client.Connect("127.0.0.1", 60000);

	bool key[3] = { false, false, false };
	bool oldKey[3] = { false, false, false };

	bool bQuit = false;
	while (!bQuit)
	{
		if (GetForegroundWindow() == GetConsoleWindow())
		{
			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
		}

		if (key[0] && !oldKey[0])
			client.PingServer();
		if (key[1] && !oldKey[1])
			client.MessageAll();
		if (key[2] && !oldKey[2])
			bQuit = true;

		for (int i = 0; i < 3; ++i)
		{
			oldKey[i] = key[i];
		}

		if (client.IsConnected())
		{
			if (!client.GetIncomingMessages().empty())
			{
				auto message = client.GetIncomingMessages().pop_front().m_Message;

				switch (message.m_Header.m_ID)
				{
					case CustomMessageTypes::ServerAccept:
					{
						std::cout << "Server Accepted Connection\n";
						break;
					}
					case CustomMessageTypes::ServerPing:
					{
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point timeThen;
						message >> timeThen;
						std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
						break;
					}
					case CustomMessageTypes::ServerMessage:
					{
						// Server has responded.
						uint32_t clientID;
						message >> clientID;
						std::cout << "Hello from [" << clientID << "]\n";
						break;
					}
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}
	}

	return 0;
}