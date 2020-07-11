#pragma once
#include "TuranAPI/API_includes.h"


/*
	Create a Profiling system that starts profiling in Begin_Profiling, profiles functions with Profile_Function, profiles scopes with Profile_Scope(name) and ends all profiling systems with End_Profiling!


*/


namespace TuranAPI {
		//Store a specific function or scope in this class!

	class TURANAPI Profiled_Scope {
	public:
		bool Is_Recording;
		long long START_POINT, END_POINT, THREAD_ID, DURATION;
		string NAME;
		//Use this constructor to fill the data later!
		Profiled_Scope();
		//Use this constructor to start profiling a scope!
		Profiled_Scope(const char* name);
		~Profiled_Scope();
	};

	//Store recording data in this class!

	class TURANAPI Profiling_Session {
	public:
		string FILEPATH;
		vector<Profiled_Scope>* PROFILED_SCOPEs_vector;

		Profiling_Session(const char* PATH);
		void FILL_PROFILING_DATA(Profiled_Scope* profiled_scopes_array, const unsigned int scope_number);
		~Profiling_Session();
	};

	//Use this class for recording!
	//Only one recording is possible!

	class TURANAPI Active_Profiling_Session : Profiling_Session {
		friend class TAPI_Systems;
		Active_Profiling_Session(const char* FILEPATH);
		void* START_POINT_PTR;
	public:
		static Profiling_Session* SELF;
		~Active_Profiling_Session();

		friend TURANAPI void Stop_Recording_Session();
		friend TURANAPI void Save_a_ProfiledScope_toSession(const Profiled_Scope& PROFILED_SCOPE);
	};

	TURANAPI void Save_a_ProfiledScope_toSession(const Profiled_Scope& PROFILED_SCOPE);
	TURANAPI Profiling_Session* Load_a_Profiling_Resource(const char* PATH);
	TURANAPI void Save_a_ProfilingResource_toDisk(const Profiling_Session& Resource);
}

#define TURAN_STOP_PROFILING() TuranAPI::Stop_Recording_Session()
#define TURAN_PROFILE_SCOPE(name) TuranAPI::Profiled_Scope ProfilingScope##__LINE__(name)
#define TURAN_PROFILE_FUNCTION() TURAN_PROFILE_SCOPE(__FUNCSIG__)
