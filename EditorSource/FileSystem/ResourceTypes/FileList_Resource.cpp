#include "FileList_Resource.h"
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "Scene_Resource.h"
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"

namespace TuranEditor {
	FileList_Resource::FileList_Resource(const char* path) : PATH(path), ID_BITSET(100, LASTUSEDALLOCATOR) {
	}

	vector<Resource_Identifier*>* FileList_Resource::Get_ContentListvector() {
		return &ContentListvector;
	}

	bool FileList_Resource::Verify_Resource_Data() {
		if (!PATH.length()) {
			TuranAPI::LOG_STATUS(PATH);
			TuranAPI::Breakpoint("File List Resource isn't verified because it has invalid Path!");
			return false;
		}
		return true;
	}

	void FileList_Resource::Load_FromDisk(IAllocator* Allocator) {
		void* File_List_Data = nullptr;
		//No usage for now!
		unsigned int data_size;
		File_List_Data = TAPIFILESYSTEM::Read_BinaryFile(PATH.c_str(), data_size, Allocator);
		while (!File_List_Data) {
			std::cout << "Please enter a new valid PATH: ";
			std::string PATH_str;
			std::cin >> PATH_str;
			PATH = PATH_str.c_str();
			File_List_Data = TAPIFILESYSTEM::Read_BinaryFile(PATH.c_str(), data_size, Allocator);
		}

		auto RESOURCE_typeless = EditorAsset::GetResource(File_List_Data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! File List isn't a valid resource!\n";
			return;
		}
		auto File_List = RESOURCE_typeless->TYPE_as_File_List();

		if (!File_List) {
			std::cout << "Type isn't File List, Type definition is wrong!\n";
			return;
		}
		auto File_vector = File_List->FILE_LIST();
		

		//For each file that is in File_List.enginecont!
		for (unsigned int i = 0; i < File_vector->Length(); i++) {
			auto FILE = File_vector->Get(i);

			const char* PATH = FILE->PATH()->c_str();
			auto TYPE = FILE->TYPE();
			size_t Asset_ID = FILE->ID();

			void* data = nullptr;
			unsigned int data_size;
			std::cout << "Reading the file: " << PATH << std::endl;
			data = TAPIFILESYSTEM::Read_BinaryFile(PATH, data_size, Allocator);

			//If compiled resource isn't found!
			if (data == nullptr) {
				std::cout << "Resource isn't found in path: " << PATH << std::endl;
				continue;
			}
			Resource_Identifier* loaded_resource = nullptr;
			std::cout << "Data size: " << data_size << std::endl;

			Read_ID(Asset_ID);
			loaded_resource->ID = Asset_ID;
			loaded_resource->PATH = PATH;
			loaded_resource->TYPE = Convert_fromFB_ResourceType(TYPE);
			loaded_resource->DATA = nullptr;
			ContentListvector.push_back(loaded_resource);
		}

	}
	EditorAsset::File_Type Convert_toFB_ResourceType(RESOURCETYPEs resource_type);
	RESOURCETYPEs Convert_fromFB_ResourceType(EditorAsset::File_Type resource_type);
	void FileList_Resource::Write_ToDisk() {
		//Create a flatbufferbuilder and FileList to build the data!
		flatbuffers::FlatBufferBuilder builder(1024);


		//Read each Resource_File struct and create a File struct for each of them!
		//Store each created File struct in a vector to set FileList's vector!
		std::vector<flatbuffers::Offset<EditorAsset::File>> Resources;
		for (size_t i = 0; i < Get_ContentListvector()->size(); i++) {
			Resource_Identifier* RESOURCE = (*Get_ContentListvector())[i];
			if (RESOURCE == nullptr) {
				continue;
			}
			std::cout << "Adding resource to GameContent List: " << RESOURCE->PATH << std::endl;
			auto PATH = builder.CreateString(RESOURCE->PATH);
			EditorAsset::FileBuilder file_build(builder);
			file_build.add_PATH(PATH);
			file_build.add_TYPE(Convert_toFB_ResourceType(RESOURCE->TYPE));
			std::cout << "ID: " << RESOURCE->ID << std::endl;
			file_build.add_ID(RESOURCE->ID);
			auto finished_file = file_build.Finish();
			Resources.push_back(finished_file);
			std::cout << "Added resource to GameContent List: " << RESOURCE->PATH << std::endl;
		}
		auto finished_vector = builder.CreateVector(Resources);

		auto finished_filelist = EditorAsset::CreateFileList(builder, finished_vector, 0);
		//Create, initialize and finish editing the File_List!
		auto finished_RESOURCE = EditorAsset::CreateResource(builder, EditorAsset::Resource_Type_File_List, finished_filelist.Union());
		builder.Finish(finished_RESOURCE);

		//Get data pointer and data size from builder!
		void* data = builder.GetBufferPointer();
		unsigned int data_size = builder.GetSize();

		//Overwrite the File_List with new File_List!
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH.c_str(), data, data_size);
	}





	//Asset ID's should be bigger than 0, 0 is invalid value for Asset ID's.
	//But Bitset starts from index 0, so we should add 1.
	size_t FileList_Resource::Create_Resource_ID() {
		size_t availableID = ID_BITSET.GetIndex_FirstFalse() + 1;
		ID_BITSET.SetBit_True(availableID - 1);
		return availableID;
	}
	void FileList_Resource::Read_ID(unsigned int Asset_ID) {
		ID_BITSET.SetBit_True(Asset_ID - 1);
	}
	void FileList_Resource::Delete_Resource_ID(unsigned int Asset_ID) {
		ID_BITSET.SetBit_False(Asset_ID - 1);
	}
}