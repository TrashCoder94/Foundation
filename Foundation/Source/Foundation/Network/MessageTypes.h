#pragma once

namespace Foundation
{
	// 8 bytes
	enum class NetMessageTypes : uint32_t
	{
		ServerAccept,
		ServerDeny,
		ServerPing,
		ServerMessage,
		MessageAll
	};
}