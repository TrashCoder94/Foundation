#include "ServerApplication.h"

namespace Foundation
{
	ServerApplication* ServerApplication::s_pInstance = nullptr;

	ServerApplication::ServerApplication(const uint16_t port, const std::string& name) : Server(port),
		m_Name(name),
		m_Running(true)
	{
		FD_PROFILE_FUNCTION();

		FD_CORE_ASSERT(!s_pInstance, "Server application already exists!");
		s_pInstance = this;

		Start();
	}

	ServerApplication::~ServerApplication()
	{
		m_Running = false;
	}

	void ServerApplication::Run()
	{
		while (m_Running)
		{
			Update(-1, true);
		}
	}
}