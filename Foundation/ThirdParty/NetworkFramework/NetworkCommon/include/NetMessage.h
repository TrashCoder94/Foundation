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

namespace Net
{
	// Message header is sent at start of all messages. The template allows us
	// to use "enum class" to ensure that the messages are valid at compile time
	template<typename T>
	struct MessageHeader
	{
		T m_ID{};
		uint32_t m_Size{ 0 };
	};

	template<typename T>
	struct Message
	{
		MessageHeader<T> m_Header{};
		std::vector<uint8_t> m_Body;

		// Return size of entire message packet in bytes.
		size_t size() const
		{
			return m_Body.size();
		}

		friend std::ostream& operator << (std::ostream& os, const Message<T>& message)
		{
			os << "ID:" << int(message.m_Header.m_ID) << " Size: " << message.m_Header.m_Size;
			return os;
		}

		template<typename DataType>
		friend Message<T>& operator << (Message<T>& message, const DataType& data)
		{
			// Check that the type of the data being pushed is trivially copyable.
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into message.");

			// Cache current size of vector, as this will be the point we insert the data.
			size_t i = message.m_Body.size();

			// Resize the vector by the size of the data being pushed
			message.m_Body.resize(message.m_Body.size() + sizeof(DataType));

			// Copy the data into the newly allocated vector space.
			std::memcpy(message.m_Body.data() + i, &data, sizeof(DataType));

			// Recalculate the size of the message.
			message.m_Header.m_Size = (uint32_t)message.size();

			// Return our message so we can chain it.
			return message;
		}

		template<typename DataType>
		friend Message<T>& operator >> (Message<T>& message, DataType& data)
		{
			// Check that the type of the data being pushed is trivially copyable.
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into message.");

			// Cache the location towards the end of the vector where the pulled data starts.
			size_t i = message.m_Body.size() - sizeof(DataType);

			// Copy the data into the newly allocated vector space.
			std::memcpy(&data, message.m_Body.data() + i, sizeof(DataType));

			// Reduce the size of the vector.
			message.m_Body.resize(i);

			// Recalculate the size of the message.
			message.m_Header.m_Size = (uint32_t)message.size();

			// Return our message so we can chain it.
			return message;
		}
	};

	template<typename T>
	class Connection;

	template<typename T>
	struct OwnedMessage
	{
		std::shared_ptr<Connection<T>> m_Remote = nullptr;
		Message<T> m_Message;

		friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& message)
		{
			os << message.message;
			return os;
		}
	};
}