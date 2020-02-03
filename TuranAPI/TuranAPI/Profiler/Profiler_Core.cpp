#include "Profiler_Core.h"
#include "TuranAPI/API_FileSystem.h"
#include <flatbuffers/flatbuffers.h>
#include "TuranAPI/FileSystem/DataFormats/TuranAPIFormats_generated.h"
using namespace TuranAPI::Profiler;

//CODE ALL OF THESE!
Profiled_Scope::Profiled_Scope(){}
Profiled_Scope::Profiled_Scope(const string& name) : NAME(name) {
	START_POINT = chrono::time_point_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()).time_since_epoch().count();
	THREAD_ID = hash<thread::id>{}(this_thread::get_id());
	Is_Recording = true;
}
Profiled_Scope::~Profiled_Scope() {
	//If the scope was being recorded!
	if (Is_Recording) {
		END_POINT = chrono::time_point_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()).time_since_epoch().count();
		DURATION = END_POINT - START_POINT;

		Save_a_ProfiledScope_toSession(*this);
	}
	//If the scope profiling data is filled from a file!
	else {

	}
}




Profiling_Session::Profiling_Session(const string& PATH) : FILEPATH(PATH) {}
Profiling_Session::~Profiling_Session() {}
void Profiling_Session::FILL_PROFILING_DATA(Profiled_Scope* profiled_scopes_array, const unsigned int scope_number) {
	for (unsigned int i = 0; i < scope_number; i++) {
		PROFILED_SCOPEs_ARRAY[i] = profiled_scopes_array[i];
	}
}


Profiling_Session* Active_Profiling_Session::SELF = nullptr;
void* Active_Profiling_Session::START_POINT_PTR = nullptr;
Active_Profiling_Session::Active_Profiling_Session(const string& FILEPATH) : Profiling_Session(FILEPATH){}
Active_Profiling_Session::~Active_Profiling_Session() {
}



void TuranAPI::Profiler::Begin_Recording_Session(const string& FILEPATH) {
	Active_Profiling_Session::START_POINT_PTR = new chrono::time_point<chrono::steady_clock>;
	*((chrono::time_point<chrono::steady_clock>*)Active_Profiling_Session::START_POINT_PTR) = chrono::high_resolution_clock::now();


	Active_Profiling_Session::SELF = new Profiling_Session(FILEPATH);
}

void TuranAPI::Profiler::Stop_Recording_Session() {
	Save_a_ProfilingResource_toDisk(*(Active_Profiling_Session::SELF));

	delete Active_Profiling_Session::START_POINT_PTR;
	Active_Profiling_Session::START_POINT_PTR = nullptr;
	Active_Profiling_Session::SELF = nullptr;
}


Profiling_Session* TuranAPI::Profiler::Load_a_Profiling_Resource(const string& PATH) {
	Profiling_Session* Profiled_Session = new Profiling_Session(PATH);

	//Read the Profiling File!
	unsigned int data_size;
	void* Profiling_Resource_BinaryData = TuranAPI::File_System::FileSystem::Read_BinaryFile(PATH, data_size);
	auto RESOURCE_typeless = TuranAPIFormats::GetResource(Profiling_Resource_BinaryData);
	if (RESOURCE_typeless == nullptr) {
		cout << "Error: Loading failed! Profiling Resource isn't a valid resource!\n";
		return nullptr;
	}
	auto Profiling_Resource = RESOURCE_typeless->TYPE_as_Profiling();

	const unsigned int PROFILEDSCOPE_NUMBER = Profiling_Resource->Profiled_Scopes()->size();
	//Get the profiling data from the flatbuffer data!
	vector<Profiled_Scope> Profiled_Scopes_Array;
	for (unsigned int i = 0; i < PROFILEDSCOPE_NUMBER; i++) {
		Profiled_Scope Current_ProfiledScope;
		Current_ProfiledScope.START_POINT = Profiling_Resource->Profiled_Scopes()->Get(i)->START_POINT();
		Current_ProfiledScope.END_POINT = Profiling_Resource->Profiled_Scopes()->Get(i)->END_POINT();
		Current_ProfiledScope.THREAD_ID = Profiling_Resource->Profiled_Scopes()->Get(i)->THREAD_ID();
		Current_ProfiledScope.DURATION = Current_ProfiledScope.END_POINT - Current_ProfiledScope.START_POINT;
		Current_ProfiledScope.NAME = Profiling_Resource->Profiled_Scopes()->Get(i)->NAME()->c_str();
		Profiled_Scopes_Array.push_back(Current_ProfiledScope);
	}
	Profiled_Session->PROFILED_SCOPEs_ARRAY = Profiled_Scopes_Array;


	return Profiled_Session;
}

void TuranAPI::Profiler::Save_a_ProfilingResource_toDisk(const Profiling_Session& Resource) {
	//Create a flatbufferbuilder to build the data!
	flatbuffers::FlatBufferBuilder builder(1024);

	//Create and fill with data a vector of Profiled Scopes!
	vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>> Profiled_Scopes;
	for (unsigned int i = 0; i < Resource.PROFILED_SCOPEs_ARRAY.size(); i++) {
		auto NAME = builder.CreateString(Resource.PROFILED_SCOPEs_ARRAY[i].NAME);
		TuranAPIFormats::Profiled_ScopeBuilder Scope_Builder(builder);
		Scope_Builder.add_START_POINT(Resource.PROFILED_SCOPEs_ARRAY[i].START_POINT);
		Scope_Builder.add_END_POINT(Resource.PROFILED_SCOPEs_ARRAY[i].END_POINT);
		Scope_Builder.add_THREAD_ID(Resource.PROFILED_SCOPEs_ARRAY[i].THREAD_ID);
		cout << "I'm HERE!\n";
		cout << Resource.PROFILED_SCOPEs_ARRAY[i].NAME << endl;
		Scope_Builder.add_NAME(NAME);
		auto finished_Scope_Data = Scope_Builder.Finish();
		Profiled_Scopes.push_back(finished_Scope_Data);
	}
	auto finished_Profiled_Scopes_Array = builder.CreateVector(Profiled_Scopes);
	cout << "Finished creating the profiled scopes vector!\n";


	//Create, initialize and finish editing the Profiling Session Flatbuffer Data!
	auto finished_profiling_data = TuranAPIFormats::CreateProfiling_Session(builder, finished_Profiled_Scopes_Array);
	auto finished_RESOURCE = TuranAPIFormats::CreateResource(builder, TuranAPIFormats::Resource_Type_Profiling, finished_profiling_data.Union());
	builder.Finish(finished_RESOURCE);

	//Get data pointer and data size from builder!
	void* data = builder.GetBufferPointer();
	unsigned int data_size = builder.GetSize();

	//Overwrite the File_List with new Profiling Session File!
	TuranAPI::File_System::FileSystem::Overwrite_BinaryFile(Resource.FILEPATH, data, data_size);
	cout << "Overwritten!\n";
}

void TuranAPI::Profiler::Save_a_ProfiledScope_toSession(const Profiled_Scope& PROFILED_SCOPE) {
	Active_Profiling_Session::SELF->PROFILED_SCOPEs_ARRAY.push_back(PROFILED_SCOPE);
	cout << "A Profiled scope is saved!" << endl;
	cout << "Scope Name:" << PROFILED_SCOPE.NAME << endl;
	cout << "Scope Start Point: " << PROFILED_SCOPE.START_POINT << endl;
	cout << "Scope End Point: " << PROFILED_SCOPE.END_POINT << endl;
	cout << "Scope Duration: " << PROFILED_SCOPE.DURATION << endl;
	cout << "Scope Thread ID: " << PROFILED_SCOPE.THREAD_ID << endl;
}








/*

Timer* Profiler_Core::Start_Timing() {
	auto DATA_POINTER_CREATION = new chrono::time_point<chrono::steady_clock>;
	Timer* Timing = new Timer;
	Timing->START_POINT_PTR = (void*)DATA_POINTER_CREATION;

	*(chrono::time_point<chrono::steady_clock>*)Timing->START_POINT_PTR = chrono::high_resolution_clock::now();
	return Timing;
}

float Profiler_Core::Get_Timing_in_Seconds(Timer* Timing) {
	if (Timing == nullptr || Timing->START_POINT_PTR == nullptr) {
		cout << "Error: You forgot the call Start_Timing() before Get_Timing()!\n";
		SLEEP_THREAD(20);
		return 0.0f;
	}
	if (Timing->PAUSE_POINT_PTRs.size() != Timing->CONTINUE_POINT_PTRs.size()) {
		cout << "Error: Pause point number isn't doesn't match the continue point number!\n";
		SLEEP_THREAD(20);
		return 0.0f;
	}
	auto NOW = chrono::high_resolution_clock::now();

	//First, calculate the time between pause-continues
	chrono::duration<float> paused_duration;
	for (unsigned int i = 0; i < Timing->PAUSE_POINT_PTRs.size(); i++) {
		paused_duration += ((*(chrono::time_point<chrono::steady_clock>*)Timing->CONTINUE_POINT_PTRs[i]) - (*(chrono::time_point<chrono::steady_clock>*)Timing->PAUSE_POINT_PTRs[i]));
	}
	//Then calculate the the total duration
	chrono::duration<float> duration = NOW - (*(chrono::time_point<chrono::steady_clock>*)Timing->START_POINT_PTR) - paused_duration;
	return duration.count();
}

void Profiler_Core::Pause_Timing(Timer* Timing) {
	if (Timing == nullptr || Timing->START_POINT_PTR == nullptr) {
		cout << "Error: You forgot the call Start_Timing() before Pause_Timing()!\n";
		SLEEP_THREAD(20);
		return;
	}
	if (Timing->IS_PAUSED) {
		cout << "Error: Timer is already paused, You can't pause twice before you continue!\n";
		SLEEP_THREAD(20);
		return;
	}

	Timing->PAUSE_POINT_PTRs.push_back(new chrono::time_point<chrono::steady_clock>(chrono::high_resolution_clock::now()));
}

void Profiler_Core::Continue_Timing(Timer* Timing) {
	if (Timing->START_POINT_PTR == nullptr) {
		cout << "Error: You forgot the call Start_Timing() before Pause_Timing()!\n";
		SLEEP_THREAD(20);
		return;
	}
	if (!Timing->IS_PAUSED) {
		cout << "Error: You can't continue, because timer is already continuing!\n";
		SLEEP_THREAD(20);
		return;
	}

	Timing->CONTINUE_POINT_PTRs.push_back(new chrono::time_point<chrono::steady_clock>(chrono::high_resolution_clock::now()));
}

void Profiler_Core::End_Timing(Timer* Timing) {
	delete Timing;
	Timing = nullptr;
}
*/