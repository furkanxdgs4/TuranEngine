#pragma once
#include "TuranAPI/API_includes.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"

namespace TuranAPI {
	namespace Logging {
		enum class LOG_TYPE : char {
			CRASHING_ERROR = 0, ERROR = 1, WARNING = 2, STATUS = 3, NOT_CODEDPATH = 4
		};

		struct LOG {
		public:
			LOG_TYPE TYPE;
			String TEXT;
		};

		class TURANAPI Logger {
			String MainLogFile_Path;
			String WarningLogFile_Path;
			String ErrorLogFile_Path;
			String NotCodedLogFile_Path;
			Vector<LOG> LOGs;
		public:
			static Logger* SELF;
			Logger();
			~Logger();
			void Write_LOGs_toTextFiles();
			void Log_CrashingError(const char* log);
			void Log_HandledError(const char* log);
			void Log_Warning(const char* log);
			void Log_Status(const char* log);
			void Log_NotCodedPath(const char* log, bool stop_running);
		};

	}
#define TURAN_DEBUGGING
#ifdef TURAN_DEBUGGING
#define LOG_CRASHING(LOG_STRING)							Logging::Logger::SELF->Log_CrashingError(LOG_STRING)
#define LOG_ERROR(LOG_STRING)								Logging::Logger::SELF->Log_HandledError(LOG_STRING)
#define LOG_WARNING(LOG_STRING)								Logging::Logger::SELF->Log_Warning(LOG_STRING)
#define LOG_STATUS(LOG_STRING)								Logging::Logger::SELF->Log_Status(LOG_STRING)
#define LOG_NOTCODED(LOG_STRING, Stop_Application_BOOL)		Logging::Logger::SELF->Log_NotCodedPath(LOG_STRING, Stop_Application_BOOL)
#define WRITE_LOGs_toFILEs()								Logging::Logger::SELF->Write_LOGs_toTextFiles()
#else
#define LOG_CRASHING(LOG_STRING) Empty()
#define LOG_ERROR(LOG_STRING) Empty()
#define LOG_WARNING(LOG_STRING) Empty()
#define LOG_STATUS(LOG_STRING) Empty()
#define LOG_NOTCODED(LOG_STRING, Stop_Application_BOOL) Empty()
#define WRITE_LOGs_toFILEs() Empty()
#endif
}