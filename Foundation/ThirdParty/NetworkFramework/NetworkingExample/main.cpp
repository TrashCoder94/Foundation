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

#include <chrono>
#include <iostream>
#include <string>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

// 1kb buffer
std::vector<char> buffer(1 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	// When some data arrives at this socket.
	// It will execute the lamda here.
	socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (size_t i = 0; i < length; ++i)
					std::cout << buffer[i];

				GrabSomeData(socket);
			}
		});
}

int main()
{
	asio::error_code ec;

	// Create a "context" - essentially the platform specific interface.
	asio::io_context context;

	// Give some fake tasks for asio so that the context doesn't finish straight away.
	asio::io_context::work idleWork(context);

	// Start the context in it's own thread.
	// So if it has to stop and wait it doesn't block the main thread.
	std::thread contextThread = std::thread([&]() { context.run(); });

	// Get the address of somewhere we wish to connect to
	// Attempting to connect to http://example.com/
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);
	
	//asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 80);

	// Create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{
		// Setup async functions before we write any bytes.
		GrabSomeData(socket);

		// HTML request
		std::string request =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		// Write some data to this socket.
		socket.write_some(asio::buffer(request.data(), request.size()), ec);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);

		context.stop();
		if (contextThread.joinable())
			contextThread.join();

		//socket.wait(socket.wait_read);

		//// See if we have any bytes available from the socket.
		//size_t bytes = socket.available();
		//std::cout << "Bytes Available: " << bytes << std::endl;

		//if (bytes > 0)
		//{
		//	std::vector<char> buffer(bytes);
		//	socket.read_some(asio::buffer(buffer.data(), buffer.size()), ec);

		//	for (auto& c : buffer)
		//		std::cout << c;
		//}
	}

	system("pause");
	return 0;
}