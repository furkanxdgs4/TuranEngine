#include "Logger_Core.h"
#include "TuranAPI/API_Profiler.h"

enum class LOG_TYPE : char{
	CRASHING_ERROR = 0, ERROR = 1, WARNING = 2, STATUS = 3, NOT_CODEDPATH = 4
};

class LOG {
public:
	LOG_TYPE TYPE;
	string LOG;
};

//Please don't move the above classes anywhere! I just want to use them here, so don't need to worry about wrong usage


using namespace TuranAPI::File_System;
using namespace TuranAPI::Logging;

string Logger::MainLogFile_Path = "C:/dev/TuranEngine/logs.txt";
string Logger::WarningLogFile_Path = "C:/dev/TuranEngine/warnings.txt";
string Logger::ErrorLogFile_Path = "C:/dev/TuranEngine/errors.txt";
string Logger::NotCodedLogFile_Path = "C:/dev/TuranEngine/notcodedpaths.txt";
vector<void*> Logger::LOGs = vector<void*>{};

void Logger::Start_LoggingSystem() {
	TuranAPI::File_System::FileSystem::Write_TextFile("TuranAPI: Logging Started!", MainLogFile_Path, false);
	TuranAPI::File_System::FileSystem::Write_TextFile("TuranAPI: Logging Started!", WarningLogFile_Path, false);
	TuranAPI::File_System::FileSystem::Write_TextFile("TuranAPI: Logging Started!", ErrorLogFile_Path, false);
	TuranAPI::File_System::FileSystem::Write_TextFile("TuranAPI: Logging Started!", NotCodedLogFile_Path, false);
}

void Logger::Write_LOGs_toTextFiles() {
	string MainLogFile_Text, ErrorLogFile_Text, WarningLogFile_Text, NotCodedLogFile_Text;

	for (void* log_dataptr : LOGs) {
		LOG* log_data = (LOG*)log_dataptr;
		
		if (log_data->TYPE == LOG_TYPE::CRASHING_ERROR || log_data->TYPE == LOG_TYPE::ERROR) {
			MainLogFile_Text += log_data->LOG;
			ErrorLogFile_Text += log_data->LOG;
		}
		else if (log_data->TYPE == LOG_TYPE::WARNING) {
			MainLogFile_Text += log_data->LOG;
			WarningLogFile_Text += log_data->LOG;
		}
		else if (log_data->TYPE == LOG_TYPE::STATUS) {
			MainLogFile_Text += log_data->LOG;
		}
		else if (log_data->TYPE == LOG_TYPE::NOT_CODEDPATH) {
			MainLogFile_Text += log_data->LOG;
			ErrorLogFile_Text += log_data->LOG;
			NotCodedLogFile_Text += log_data->LOG;
		}

		delete (LOG*)log_dataptr;
		log_dataptr = nullptr;
	}

	
	TuranAPI::File_System::FileSystem::Write_TextFile(MainLogFile_Text, MainLogFile_Path, true);
	TuranAPI::File_System::FileSystem::Write_TextFile(ErrorLogFile_Text, ErrorLogFile_Path, true);
	TuranAPI::File_System::FileSystem::Write_TextFile(WarningLogFile_Text, WarningLogFile_Path, true);
	TuranAPI::File_System::FileSystem::Write_TextFile(NotCodedLogFile_Text, NotCodedLogFile_Path, true);

	LOGs.clear();
}

void Logger::Log_CrashingError(string log) {
	time_t current_time = time(NULL);
	string time_string = ctime(&current_time);

	LOG* log_data = new LOG;
	log_data->LOG = "\n\n" + time_string + " CRASHING ERROR:\n" + log + "\n";
	log_data->TYPE = LOG_TYPE::CRASHING_ERROR;
	LOGs.push_back(log_data);

	cout << log_data->LOG ;

	Write_LOGs_toTextFiles();

	TuranAPI::Breakpoint();
}

void Logger::Log_HandledError(string log) {
	time_t current_time = time(NULL);
	string time_string = ctime(&current_time);

	LOG* log_data = new LOG;
	log_data->LOG = "\n\n" + time_string + " ERROR:\n" + log + "\n";
	log_data->TYPE = LOG_TYPE::ERROR;
	LOGs.push_back(log_data);

	cout << log_data->LOG;
}

void Logger::Log_Warning(string log) {
	time_t current_time = time(NULL);
	string time_string = ctime(&current_time);

	LOG* log_data = new LOG;
	log_data->LOG = "\n\n" + time_string + " WARNING:\n" + log + "\n";
	log_data->TYPE = LOG_TYPE::WARNING;
	LOGs.push_back(log_data);
	 
	cout << log_data->LOG;
}

void Logger::Log_Status(string log) {
	LOG* log_data = new LOG;
	log_data->LOG = log + "\n";
	log_data->TYPE = LOG_TYPE::STATUS;
	LOGs.push_back(log_data);

	cout << log_data->LOG;
}

void Logger::Log_NotCodedPath(string log, bool stop_running) {
	if (stop_running) {
		Write_LOGs_toTextFiles();
	}

	LOG* log_data = new LOG;
	log_data->LOG = log + "\n";
	log_data->TYPE = LOG_TYPE::NOT_CODEDPATH;
	LOGs.push_back(log_data);

	if (stop_running) {
		Write_LOGs_toTextFiles();
		TuranAPI::Breakpoint(log_data->LOG);
	}
}