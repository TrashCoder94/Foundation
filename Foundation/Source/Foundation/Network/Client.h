#pragma once

#include <NetCommon.h>
#include "MessageTypes.h"

namespace Foundation
{
	class Client : public Net::Client<NetMessageTypes>
	{
		public:
			void PingServer();
			void MessageAll();
	};
}