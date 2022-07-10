#pragma once

#include <Foundation.h>

int main(int argc, char** argv);

namespace Foundation
{
	class ServerApplication : public Server
	{
		public:
			ServerApplication(const uint16_t port, const std::string& name = "Foundation Server Application");
			virtual ~ServerApplication();

			static ServerApplication& Get() { return *s_pInstance; }

		private:
			void Run();

		private:
			std::string m_Name;
			bool m_Running;

			static ServerApplication* s_pInstance;

			// Used so we can keep Run() private.
			// This may need to change if there are any updates to EntryPoint.h
			friend int ::main(int argc, char** argv);
	};
}