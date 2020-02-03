#pragma once
#include "TuranAPI/API_includes.h"

//Resource Types to present data in API!
#include "Resource_Types/Resource_Type.h"
#include "Resource_Types/Material_Type_Resource.h"
#include "Resource_Types/FileList_Resource.h"

/*
	1) This class is to add data types to Engine!
	2) An added data type should be type of .gamecont or .enginecont
*/
namespace TuranAPI {
	namespace File_System {

		/*
		Handles all of the Resource I/O operations!
		If you want to add a data format, or write/read a file, modify this class!
		*/
		class TURANAPI FileSystem {
		protected:
			static unsigned int LAST_ID;
		public:
			//All of the File Systems should specify this functions, but all of them probably will have same functionality!
			//Just logging text will be different, I think

			//Add a content to file list, save file list to disk, save resource to disk!
			virtual void Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) = 0;
			virtual void Remove_Content_fromFileList(unsigned int index) = 0;
			virtual const vector<TuranAPI::File_System::Resource_Type*>* Get_Const_FileListContentVector() = 0;
			virtual unsigned int Get_LengthOf_FileListContentVector() = 0;

			//Read File_List.usercont to load the project!
			virtual void Load_FileListContents_fromDisk() = 0;
			//Clear all of the resource list! But resource files (.gamecont) aren't deleted.
			virtual void Clear_FileListContents() = 0;

			//Create resource ID for new imported (compiled) resources!
			static unsigned int Create_Resource_ID();

			static void* Read_BinaryFile(string path, unsigned int& size);
			static void Write_BinaryFile(string path, void* data, unsigned int size);
			static void Overwrite_BinaryFile(string path, void* data, unsigned int size);
			static void Delete_File(string path);


			static string Read_TextFile(string path);
			static void Write_TextFile(string text, string path, bool write_to_end);

			//Add resource types here!
			//These functions will be defined in Resource_Loaders folder!
			//These functions load compiled resources to memory

			static Resource_Type* Load_Model(void* data, unsigned int id, const string& path);
			static Resource_Type* Load_Material_Type(void* data, unsigned int id, const string& path);
			static Resource_Type* Load_Material_Inst(void* data, unsigned int id, const string& path);
			static Resource_Type* Load_Texture(void* data, unsigned int id, const string& path);
			static Resource_Type* Load_Scene(void* data, unsigned int id, const string& path);



			static unsigned int Get_LAST_ID();

			//Save a resource to disk! Any type is possible (GFX_Default, Engine_Default, User etc.)
			//Don't forget to update your resource list (if you have one)!
			static void Write_a_Resource_toDisk(Resource_Type* resource_data, bool Verify_ResourceData = true);

			//Load resources from a FileList to a vector!
			static void Load_Resources_fromFileList(FileList_Resource* Empty_File_List);
		};
	}
}
