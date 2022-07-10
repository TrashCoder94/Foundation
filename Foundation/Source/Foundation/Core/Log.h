#pragma once

// Hides warnings from external includes.
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Foundation
{
	class Log
	{
		public:
			static void Init();
			static std::string GetString();
			static void Clear();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_pCoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return s_pApplicationLogger; }

		private:
			static std::shared_ptr<spdlog::logger> s_pCoreLogger;
			static std::shared_ptr<spdlog::logger> s_pApplicationLogger;
			static std::ostringstream s_OutputStream;
	};
}

// Logs which will be used within the engine.
#define FD_CORE_LOG_TRACE(...)		::Foundation::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FD_CORE_LOG_INFO(...)		::Foundation::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FD_CORE_LOG_WARN(...)		::Foundation::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FD_CORE_LOG_ERROR(...)		::Foundation::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FD_CORE_LOG_CRITICAL(...)   ::Foundation::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Logs which will be used for applications which use this engine (e.g. for clients).
#define FD_LOG_TRACE(...)        ::Foundation::Log::GetApplicationLogger()->trace(__VA_ARGS__)
#define FD_LOG_INFO(...)         ::Foundation::Log::GetApplicationLogger()->info(__VA_ARGS__)
#define FD_LOG_WARN(...)         ::Foundation::Log::GetApplicationLogger()->warn(__VA_ARGS__)
#define FD_LOG_ERROR(...)        ::Foundation::Log::GetApplicationLogger()->error(__VA_ARGS__)
#define FD_LOG_CRITICAL(...)     ::Foundation::Log::GetApplicationLogger()->critical(__VA_ARGS__)
