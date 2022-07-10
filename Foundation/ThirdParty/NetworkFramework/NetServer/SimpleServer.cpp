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

class CustomServer : public Net::Server<CustomMessageTypes>
{
public:
	CustomServer(uint16_t port) : Net::Server<CustomMessageTypes>(port)
	{}

protected:
	// Called when a client connects, you can veto the connection by returning false.
	bool OnClientConnect(const std::shared_ptr<Net::Connection<CustomMessageTypes>>& client) override
	{
		Net::Message<CustomMessageTypes> message;
		message.m_Header.m_ID = CustomMessageTypes::ServerAccept;
		client->Send(message);

		return true;
	}

	// Called when a client appears to have disconnected.
	void OnClientDisconnect(const std::shared_ptr<Net::Connection<CustomMessageTypes>>& client) override
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives.
	void OnMessage(std::shared_ptr<Net::Connection<CustomMessageTypes>>& client, const Net::Message<CustomMessageTypes>& message) override
	{
		switch (message.m_Header.m_ID)
		{
			case CustomMessageTypes::ServerPing:
			{
				std::cout << "[" << client->GetID() << "]: Server Ping\n";

				client->Send(message);
				break;
			}
			case CustomMessageTypes::MessageAll:
			{
				std::cout << "[" << client->GetID() << "]: Message All\n";

				Net::Message<CustomMessageTypes> message;
				message.m_Header.m_ID = CustomMessageTypes::ServerMessage;
				message << client->GetID();
				MessageAllClients(message, client);
				break;
			}
		}
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (true)
	{
		server.Update(-1, true);
	}

	return 0;
}