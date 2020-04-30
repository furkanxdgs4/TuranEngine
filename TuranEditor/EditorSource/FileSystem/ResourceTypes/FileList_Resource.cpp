#include "FileList_Resource.h"
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "Scene_Resource.h"
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"

namespace TuranEditor {
	FileList_Resource::FileList_Resource(const char* path) : ContentListVector(LASTUSEDALLOCATOR, 4, 4), PATH(path), ID_BITSET(100, LASTUSEDALLOCATOR) {
	}

	Vector<TuranAPI::Resource_Type*>* FileList_Resource::Get_ContentListVector() {
		return &ContentListVector;
	}

	bool FileList_Resource::Verify_Resource_Data() {
		if (PATH.length() != 0) {
			for (size_t i = 0; i < ContentListVector.size(); i++) {
				TuranAPI::Resource_Type* Resource = ContentListVector[i];
				if (!Resource->Verify_Resource_Data()) {
					TuranAPI::Breakpoint("File List Resource isn't verified because a Resource isn't verified!");
					return false;
				}
			}
		}
		else {
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
		File_List_Data = TAPIFILESYSTEM::Read_BinaryFile(PATH, data_size, Allocator);
		while (!File_List_Data) {
			std::cout << "Please enter a new valid PATH: ";
			std::string PATH_str;
			std::cin >> PATH_str;
			PATH = PATH_str.c_str();
			File_List_Data = TAPIFILESYSTEM::Read_BinaryFile(PATH, data_size, Allocator);
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
		auto File_Vector = File_List->FILE_LIST();
		
		//For each file that is in File_List.enginecont!
		for (unsigned int i = 0; i < File_Vector->Length(); i++) {
			auto FILE = File_Vector->Get(i);

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
			TuranAPI::Resource_Type* loaded_resource = nullptr;
			std::cout << "Data size: " << data_size << std::endl;

			Read_ID(Asset_ID);

			//If the resource is valid!
			switch (TYPE) {
			case EditorAsset::File_Type_Static_Model:
				loaded_resource = new Static_Model_Data(0);
				loaded_resource->Load_FromDisk(data, Asset_ID, PATH, LASTUSEDALLOCATOR);
				break;
			case EditorAsset::File_Type_Texture:
				loaded_resource = new GFX_API::Texture_Resource;
				loaded_resource->Load_FromDisk(data, Asset_ID, PATH, LASTUSEDALLOCATOR);
				delete data;
				break;
			case EditorAsset::File_Type_Material_Type:
				loaded_resource = new GFX_API::Material_Type_Resource;
				loaded_resource->Load_FromDisk(data, Asset_ID, PATH, LASTUSEDALLOCATOR);
				break;
			case EditorAsset::File_Type_Material_Instance:
				loaded_resource = new GFX_API::Material_Instance;
				loaded_resource->Load_FromDisk(data, Asset_ID, PATH, LASTUSEDALLOCATOR);
				break;
			case EditorAsset::File_Type_Scene:
				TuranAPI::LOG_ERROR("Asset Type: Scene isn't supported to load for now!");
				break;
			default:
				TuranAPI::Breakpoint("Loaded file's type isn't supported, but this shouldn't have happened! Problem maybe the exporting progress!");
				break;;
			}

			if (loaded_resource != nullptr) {
				if (!loaded_resource->Verify_Resource_Data()) {
					TuranAPI::Breakpoint("There is a resource isn't verified in FileList!");
				}
				else {
					ContentListVector.push_back(loaded_resource);
				}
			}
		}

	}
	EditorAsset::File_Type Convert_to_GameCont_Type(RESOURCETYPEs resource_type);
	void FileList_Resource::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		//Create a flatbufferbuilder and FileList to build the data!
		flatbuffers::FlatBufferBuilder builder(1024);


		//Read each Resource_File struct and create a File struct for each of them!
		//Store each created File struct in a vector to set FileList's vector!
		std::vector<flatbuffers::Offset<EditorAsset::File>> Resources;
		for (size_t i = 0; i < Get_ContentListVector()->size(); i++) {
			TuranAPI::Resource_Type* RESOURCE = (*Get_ContentListVector())[i];
			if (RESOURCE == nullptr || !RESOURCE->Verify_Resource_Data()) {
				continue;
			}
			std::cout << "Adding resource to GameContent List: " << RESOURCE->PATH << std::endl;
			auto PATH = builder.CreateString(RESOURCE->PATH);
			auto RESOURCE_TYPE = RESOURCE->Get_Type();
			EditorAsset::FileBuilder file_build(builder);
			file_build.add_PATH(PATH);
			file_build.add_TYPE(Convert_to_GameCont_Type(RESOURCE->Get_Type()));
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
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH, data, data_size);
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