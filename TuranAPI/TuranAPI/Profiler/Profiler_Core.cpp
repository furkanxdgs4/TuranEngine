#include "Profiler_Core.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include <flatbuffers/flatbuffers.h>
#include "TuranAPI/FileSystem/DataFormats/TuranAPIFormats_generated.h"

namespace TuranAPI {

	//CODE ALL OF THESE!
	Profiled_Scope::Profiled_Scope() {}
	Profiled_Scope::Profiled_Scope(const char* name) : NAME(name) {
		START_POINT = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		THREAD_ID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Is_Recording = true;
	}
	Profiled_Scope::~Profiled_Scope() {
		//If the scope was being recorded!
		if (Is_Recording) {
			END_POINT = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
			DURATION = END_POINT - START_POINT;

			Save_a_ProfiledScope_toSession(*this);
		}
		//If the scope profiling data is filled from a file!
		else {

		}
	}




	Profiling_Session::Profiling_Session(const char* PATH) : FILEPATH(PATH) {}
	Profiling_Session::~Profiling_Session() {}
	void Profiling_Session::FILL_PROFILING_DATA(Profiled_Scope* profiled_scopes_array, const unsigned int scope_number) {
		for (unsigned int i = 0; i < scope_number; i++) {
			(*PROFILED_SCOPEs_VECTOR)[i] = profiled_scopes_array[i];
		}
	}


	Profiling_Session* Active_Profiling_Session::SELF = nullptr;
	Active_Profiling_Session::Active_Profiling_Session(const char* FILEPATH) : Profiling_Session(FILEPATH) {
		SELF = this;

		((Active_Profiling_Session*)Active_Profiling_Session::SELF)->START_POINT_PTR = new std::chrono::time_point<std::chrono::steady_clock>;
		*((std::chrono::time_point<std::chrono::steady_clock>*)((Active_Profiling_Session*)Active_Profiling_Session::SELF)->START_POINT_PTR) = std::chrono::high_resolution_clock::now();

		PROFILED_SCOPEs_VECTOR = new Vector<Profiled_Scope>(LASTUSEDALLOCATOR, 5, 5);
	}
	Active_Profiling_Session::~Active_Profiling_Session() {
	}


	void TuranAPI::Stop_Recording_Session() {
		Save_a_ProfilingResource_toDisk(*(Active_Profiling_Session::SELF));

		delete ((Active_Profiling_Session*)Active_Profiling_Session::SELF)->START_POINT_PTR;
		((Active_Profiling_Session*)Active_Profiling_Session::SELF)->START_POINT_PTR = nullptr;
		Active_Profiling_Session::SELF = nullptr;
	}


	Profiling_Session* TuranAPI::Load_a_Profiling_Resource(const char* PATH) {
		Profiling_Session* Profiled_Session = new Profiling_Session(PATH);

		//Read the Profiling File!
		unsigned int data_size;
		void* Profiling_Resource_BinaryData = TAPIFILESYSTEM::FileSystem::Read_BinaryFile(PATH, data_size, &TMEMORYMANAGER->TAPIAllocator);
		auto RESOURCE_typeless = TuranAPIFormats::GetResource(Profiling_Resource_BinaryData);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Profiling Resource isn't a valid resource!\n";
			return nullptr;
		}
		auto Profiling_Resource = RESOURCE_typeless->TYPE_as_Profiling();

		const unsigned int PROFILEDSCOPE_NUMBER = Profiling_Resource->Profiled_Scopes()->size();
		//Get the profiling data from the flatbuffer data!
		Profiled_Session->PROFILED_SCOPEs_VECTOR = new (LASTUSEDALLOCATOR) Vector<Profiled_Scope>(LASTUSEDALLOCATOR, 100, 100);
		for (unsigned int i = 0; i < PROFILEDSCOPE_NUMBER; i++) {
			Profiled_Scope Current_ProfiledScope;
			Current_ProfiledScope.START_POINT = Profiling_Resource->Profiled_Scopes()->Get(i)->START_POINT();
			Current_ProfiledScope.END_POINT = Profiling_Resource->Profiled_Scopes()->Get(i)->END_POINT();
			Current_ProfiledScope.THREAD_ID = Profiling_Resource->Profiled_Scopes()->Get(i)->THREAD_ID();
			Current_ProfiledScope.DURATION = Current_ProfiledScope.END_POINT - Current_ProfiledScope.START_POINT;
			Current_ProfiledScope.NAME = Profiling_Resource->Profiled_Scopes()->Get(i)->NAME()->c_str();
			Profiled_Session->PROFILED_SCOPEs_VECTOR->push_back(Current_ProfiledScope);
		}


		return Profiled_Session;
	}

	void TuranAPI::Save_a_ProfilingResource_toDisk(const Profiling_Session& Resource) {
		//Create a flatbufferbuilder to build the data!
		flatbuffers::FlatBufferBuilder builder(1024);

		//Create and fill with data a vector of Profiled Scopes!
		std::vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>> Profiled_Scopes;
		Vector<Profiled_Scope>* PROFILINGDATA_VECTOR = Resource.PROFILED_SCOPEs_VECTOR;
		for (unsigned int i = 0; i < Resource.PROFILED_SCOPEs_VECTOR->size(); i++) {
			auto NAME = builder.CreateString((*PROFILINGDATA_VECTOR)[i].NAME);
			TuranAPIFormats::Profiled_ScopeBuilder Scope_Builder(builder);
			Scope_Builder.add_START_POINT((*PROFILINGDATA_VECTOR)[i].START_POINT);
			Scope_Builder.add_END_POINT((*PROFILINGDATA_VECTOR)[i].END_POINT);
			Scope_Builder.add_THREAD_ID((*PROFILINGDATA_VECTOR)[i].THREAD_ID);
			std::cout << "I'm HERE!\n";
			std::cout << (*PROFILINGDATA_VECTOR)[i].NAME << std::endl;
			Scope_Builder.add_NAME(NAME);
			auto finished_Scope_Data = Scope_Builder.Finish();
			Profiled_Scopes.push_back(finished_Scope_Data);
		}
		auto finished_Profiled_Scopes_Array = builder.CreateVector(Profiled_Scopes);
		std::cout << "Finished creating the profiled scopes vector!\n";


		//Create, initialize and finish editing the Profiling Session Flatbuffer Data!
		auto finished_profiling_data = TuranAPIFormats::CreateProfiling_Session(builder, finished_Profiled_Scopes_Array);
		auto finished_RESOURCE = TuranAPIFormats::CreateResource(builder, TuranAPIFormats::Resource_Type_Profiling, finished_profiling_data.Union());
		builder.Finish(finished_RESOURCE);

		//Get data pointer and data size from builder!
		void* data = builder.GetBufferPointer();
		unsigned int data_size = builder.GetSize();

		//Overwrite the File_List with new Profiling Session File!
		TAPIFILESYSTEM::FileSystem::Overwrite_BinaryFile(Resource.FILEPATH, data, data_size);
		std::cout << "Overwritten!\n";
	}

	void TuranAPI::Save_a_ProfiledScope_toSession(const Profiled_Scope& PROFILED_SCOPE) {
		Active_Profiling_Session::SELF->PROFILED_SCOPEs_VECTOR->push_back(PROFILED_SCOPE);
		std::cout << "A Profiled scope is saved!" << std::endl;
		std::cout << "Scope Name:" << PROFILED_SCOPE.NAME << std::endl;
		std::cout << "Scope Start Point: " << PROFILED_SCOPE.START_POINT << std::endl;
		std::cout << "Scope End Point: " << PROFILED_SCOPE.END_POINT << std::endl;
		std::cout << "Scope Duration: " << PROFILED_SCOPE.DURATION << std::endl;
		std::cout << "Scope Thread ID: " << PROFILED_SCOPE.THREAD_ID << std::endl;
	}


}