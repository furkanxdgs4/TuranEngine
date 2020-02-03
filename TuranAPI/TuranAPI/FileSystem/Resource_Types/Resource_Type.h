#pragma once
#include "TuranAPI/API_includes.h"


namespace TuranAPI {
	namespace File_System {
		//Each resource is inherited from this!
		//For FileList resources, ID is 0!
		class TURANAPI Resource_Type
		{
		protected:
			friend class FileSystem;
		public:
			string PATH;
			unsigned int ID;
			//Name to show in Content Browser
			string NAME;

			unsigned int Get_ID();
			virtual TuranAPI_ENUMs Get_Resource_Type() = 0;
			virtual bool Verify_Resource_Data() = 0;
		};
	}
}