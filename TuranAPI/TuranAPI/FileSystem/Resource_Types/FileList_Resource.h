#pragma once
#include "TuranAPI/API_includes.h"
#include "Resource_Type.h"

namespace TuranAPI {
	namespace File_System {

		class TURANAPI FileList_Resource : public Resource_Type {
			vector<TuranAPI::File_System::Resource_Type*> All_Resources;
		public:
			FileList_Resource(const string& path);
			virtual TuranAPI::TuranAPI_ENUMs Get_Resource_Type() override;
			vector<Resource_Type*>* Get_ContentListVector();
			virtual bool Verify_Resource_Data();
		};

	}
}