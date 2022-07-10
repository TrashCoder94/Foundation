#include "fdpch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>

namespace Foundation
{
	std::shared_ptr<spdlog::logger> Log::s_pCoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_pApplicationLogger;
	std::ostringstream Log::s_OutputStream;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Foundation.log", true));
		logSinks.emplace_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(s_OutputStream));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");
		logSinks[2]->set_pattern("[%T] [%l] %n: %v");

		s_pCoreLogger = std::make_shared<spdlog::logger>("FOUNDATION", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_pCoreLogger);
		s_pCoreLogger->set_level(spdlog::level::trace);
		s_pCoreLogger->flush_on(spdlog::level::trace);

		s_pApplicationLogger = std::make_shared<spdlog::logger>("APPLICATION", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_pApplicationLogger);
		s_pApplicationLogger->set_level(spdlog::level::trace);
		s_pApplicationLogger->flush_on(spdlog::level::trace);
	}

	std::string Log::GetString()
	{
		std::string tempLog = s_OutputStream.str();
		return tempLog;
	}

	void Log::Clear()
	{
		s_OutputStream.str("");
		s_OutputStream.clear();
	}
}