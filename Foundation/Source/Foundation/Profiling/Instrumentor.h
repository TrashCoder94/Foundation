#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

#include "Foundation/Core/Log.h"

namespace Foundation
{
	struct ProfileResult
	{
		const char* m_Name;
		std::chrono::duration<double, std::micro> m_Start;
		std::chrono::microseconds m_ElapsedTime;
		std::thread::id m_ThreadID;
	};

	struct InstrumentationSession
	{
		std::string m_Name;
	};

	class Instrumentor
	{
		public:
			Instrumentor(const Instrumentor&) = delete;
			Instrumentor(Instrumentor&&) = delete;

		private:
			Instrumentor() :
				m_CurrentSession(nullptr),
				m_OutputStream()
			{
			}

			~Instrumentor()
			{
				EndSession();
			}

		public:
			void BeginSession(const std::string& name, const std::string& filepath = "results.json")
			{
				std::lock_guard lock(m_Mutex);
				if (m_CurrentSession)
				{
					// Just in case the session attempts to start before Log::Init has been called.
					if (Log::GetCoreLogger())
					{
						FD_CORE_LOG_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' is already open.", name, m_CurrentSession->m_Name);
					}
					InternalEndSession();
				}

				m_OutputStream.open(filepath);

				//
				if (m_OutputStream.is_open())
				{
					m_CurrentSession = new InstrumentationSession{ name };
					WriteHeader();
				}
				else
				{
					// Just in case the session attempts to start before Log::Init has been called.
					if (Log::GetCoreLogger())
					{
						FD_CORE_LOG_ERROR("Instrumentor could not open results file '{0}'.", filepath);
					}
				}
			}

			void EndSession()
			{
				std::lock_guard lock(m_Mutex);
				InternalEndSession();
			}

			void WriteProfile(const ProfileResult& result)
			{
				std::stringstream json("");

				std::string name = result.m_Name;
				std::replace(name.begin(), name.end(), '"', '\'');

				json << std::setprecision(3) << std::fixed;
				json << ",{";
				json << "\"cat\":\"function\",";
				json << "\"dur\":" << (result.m_ElapsedTime.count()) << ',';
				json << "\"name\":\"" << result.m_Name << "\",";
				json << "\"ph\":\"X\",";
				json << "\"pid\":0,";
				json << "\"tid\":" << result.m_ThreadID << ",";
				json << "\"ts\":" << result.m_Start.count();
				json << "}";

				std::lock_guard lock(m_Mutex);
				if (m_CurrentSession)
				{
					m_OutputStream << json.str();
					m_OutputStream.flush();
				}
			}

			static Instrumentor& Get()
			{
				static Instrumentor instance;
				return instance;
			}

		private:
			void WriteHeader()
			{
				m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
				m_OutputStream.flush();
			}

			void WriteFooter()
			{
				m_OutputStream << "]}";
				m_OutputStream.flush();
			}

			void InternalEndSession()
			{
				if (m_CurrentSession)
				{
					WriteFooter();
					m_OutputStream.close();
					delete m_CurrentSession;
					m_CurrentSession = nullptr;
				}
			}

		private:
			InstrumentationSession* m_CurrentSession;
			std::ofstream m_OutputStream;
			std::mutex m_Mutex;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) :
			m_Name(name),
			m_StartTimepoint(),
			m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			std::chrono::time_point<std::chrono::steady_clock> endTimepoint = std::chrono::steady_clock::now();
			std::chrono::duration<double, std::micro> highResStart = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils
	{
		template<size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template<size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;

			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define FD_PROFILE 0
#if FD_PROFILE
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define FD_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__DMC__) && (__DMC__ >= 0x810))
		#define FD_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define FD_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define FD_FUNC_SIG __FUNCTION__
	#elif (defined(__BORLANDC__) && (__BORDLANDC__ >= 0x550))
		#define FD_FUNC_SIG __FUNC__
	#elif (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901))
		#define FD_FUNC_SIG __func__
	#elif (defined(__cplusplus) && (__cplusplus >= 201103))
		#define FD_FUNC_SIG __func__
	#else
		#define	FD_FUNC_SIG "FD_FUNC_SIG unknown!"
	#endif

	#define FD_PROFILE_BEGIN_SESSION(name, filepath)	::Foundation::Instrumentor::Get().BeginSession(name, filepath)
	#define FD_PROFILE_END_SESSION()					::Foundation::Instrumentor::Get().EndSession()
	#define FD_PROFILE_SCOPE_LINE(name, line)			constexpr auto fixedName##line = ::Foundation::InstrumentorUtils::CleanupOutputString(name, "__cdecl");\
														::Foundation::InstrumentationTimer timer##line(fixedName##line.Data);
	#define FD_PROFILE_SCOPE(name)						FD_PROFILE_SCOPE_LINE(name, __LINE__)
	#define FD_PROFILE_FUNCTION()						FD_PROFILE_SCOPE(FD_FUNC_SIG)
#else
	#define FD_PROFILE_BEGIN_SESSION(name, filepath)
	#define FD_PROFILE_END_SESSION()
	#define FD_PROFILE_SCOPE(name)
	#define FD_PROFILE_FUNCTION()
#endif
