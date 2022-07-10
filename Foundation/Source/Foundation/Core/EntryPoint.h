#pragma once

#ifdef FD_PLATFORM_WINDOWS

extern Foundation::Application* Foundation::CreateApplication();

int main(int argc, char** argv)
{
	Foundation::Log::Init();

	FD_PROFILE_BEGIN_SESSION("Startup", "FoundationProfile-Startup.json");
	Foundation::Application* application = Foundation::CreateApplication();
	FD_PROFILE_END_SESSION();

	FD_PROFILE_BEGIN_SESSION("Runtime", "FoundationProfile-Runtime.json");
	application->Run();
	FD_PROFILE_END_SESSION();

	FD_PROFILE_BEGIN_SESSION("Shutdown", "FoundationProfile-Shutdown.json");
	delete application;
	FD_PROFILE_END_SESSION();

	return 0;
}

#endif