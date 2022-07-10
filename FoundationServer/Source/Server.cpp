#include "ServerApplication.h"

int main(int argc, char** argv)
{
	Foundation::Log::Init();

	FD_PROFILE_BEGIN_SESSION("Startup", "FoundationServerProfile-Startup.json");
	Foundation::ServerApplication* serverApplication = new Foundation::ServerApplication(60000);
	FD_PROFILE_END_SESSION();

	FD_PROFILE_BEGIN_SESSION("Runtime", "FoundationServerProfile-Runtime.json");
	serverApplication->Run();
	FD_PROFILE_END_SESSION();

	FD_PROFILE_BEGIN_SESSION("Shutdown", "FoundationServerProfile-Shutdown.json");
	delete serverApplication;
	FD_PROFILE_END_SESSION();

	return 0;
}