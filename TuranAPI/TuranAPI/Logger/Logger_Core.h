#pragma once
#include "TuranAPI/API_includes.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"

namespace TuranAPI {
	namespace Logging {

		class TURANAPI Logger {
			static string MainLogFile_Path;
			static string WarningLogFile_Path;
			static string ErrorLogFile_Path;
			static string NotCodedLogFile_Path;
			static vector<void*> LOGs;
		public:
			static void Start_LoggingSystem();
			static void Write_LOGs_toTextFiles();
			static void Log_CrashingError(string log);
			static void Log_HandledError(string log);
			static void Log_Warning(string log);
			static void Log_Status(string log);
			static void Log_NotCodedPath(string log, bool stop_running);
		};

	}
#ifdef TURAN_DEBUGGING
#define LOG_CRASHING(LOG_STRING)							Logging::Logger::Log_CrashingError(LOG_STRING)
#define LOG_ERROR(LOG_STRING)								Logging::Logger::Log_HandledError(LOG_STRING)
#define LOG_WARNING(LOG_STRING)								Logging::Logger::Log_Warning(LOG_STRING)
#define LOG_STATUS(LOG_STRING)								Logging::Logger::Log_Status(LOG_STRING)
#define LOG_NOTCODED(LOG_STRING, Stop_Application_BOOL)		Logging::Logger::Log_NotCodedPath(LOG_STRING, Stop_Application_BOOL)
#define WRITE_LOGs_toFILEs()								Logging::Logger::Write_LOGs_toTextFiles()
#else
#define LOG_CRASHING(LOG_STRING) Empty()
#define LOG_ERROR(LOG_STRING) Empty()
#define LOG_WARNING(LOG_STRING) Empty()
#define LOG_STATUS(LOG_STRING) Empty()
#define LOG_NOTCODED(LOG_STRING, Stop_Application_BOOL) Empty()
#endif
}



