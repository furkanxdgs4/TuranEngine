#include "Model_Resource.h"
#include <flatbuffers/flatbuffers.h>
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include "TuranAPI/FileSystem/Resource_Types/ResourceTYPEs.h"

namespace TuranEditor {
	Static_Mesh_Data::Static_Mesh_Data(unsigned char Attribute_Number) : DataLayout(Attribute_Number){

	}
	Static_Mesh_Data::~Static_Mesh_Data() {
	}
	bool Static_Mesh_Data::Verify_Mesh_Data() {
		if (VERTEX_NUMBER == 0){
			TuranAPI::LOG_ERROR("Vertex number is zero or below!");
			return false;
		}
		else if (!DataLayout.VerifyAttributeLayout()) {
			TuranAPI::LOG_ERROR("Attribute Layout isn't verified, so Mesh_Data isn't verified!");
			return false;
		}
		else if (!DataLayout.Does_BufferFits_Layout(DATA, DATA_SIZE, VERTEX_NUMBER)) {
			TuranAPI::LOG_ERROR("Vertex Data doesn't fit Attribute Layout!");
			return false;
		}
		return true;
	}



	//HELPER FUNCTIONs

	void GetLayout_FromFlatbuffer(const flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>* Attributes, GFX_API::VertexAttributeLayout& Layout);
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>> ConvertLayout_toFlatbuffer(const GFX_API::VertexAttributeLayout& layout, flatbuffers::FlatBufferBuilder& builder);
	GFX_API::UNIFORMTYPE GetDataType_FromFlatbuffer(EditorAsset::ATTRIBUTE_DATATYPE datatype);
	EditorAsset::ATTRIBUTE_DATATYPE ConvertDataType_ToFlatbuffer(GFX_API::UNIFORMTYPE datatype);



	//Static Model Data class function definitions

	Static_Model_Data::Static_Model_Data(size_t numberof_meshes) : Resource_Type(RESOURCETYPEs::EDITOR_STATICMODEL), MESHes(LASTUSEDALLOCATOR, 0, numberof_meshes) {

	}

	//Returns true if verified successfully
	//Checks mesh number correctness; model name's, path's, mesh array pointer's, each mesh in the array's, each vertex attribute of each mesh's existence (Is it null or wrong vs some possible value?)
	bool Static_Model_Data::Verify_Resource_Data() {
		if (MESHes.size() > 0) {
			if (NAME.length() == 0) {
				TuranAPI::LOG_ERROR("NAME is empty!");
				return false;
			}
			else if (PATH.length() == 0) {
				TuranAPI::LOG_ERROR("PATH is empty!");
				return false;
			}
			for (unsigned int mesh_index = 0; mesh_index < MESHes.size(); mesh_index++) {
				Static_Mesh_Data* MESH = MESHes[mesh_index];
				if (MESH != nullptr) {
					if (!MESH->Verify_Mesh_Data()) {
						return false;
					}
				}
			}
			TuranAPI::LOG_STATUS("All meshes are verified!");
			return true;
		}

		TuranAPI::LOG_ERROR("Mesh number is zero or below!");
		//Model isn't verified, any of the Meshes are verified.
		//There should be log!
		return false;
	}
	void Static_Model_Data::Load_FromDisk(void* data, unsigned int id, const char* path, IAllocator* Allocator) {
		std::cout << "Loading a model, ID is: " << id << std::endl;
		auto RESOURCE_typeless = EditorAsset::GetResource(data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Model isn't a valid resource!\n";
			return;
		}
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Static_Model();

		if (!RESOURCE) {
			std::cout << "Type isn't Static Model, Type definition is wrong!\n";
			return;
		}

		//Get how much material the model has!
		Material_Number = RESOURCE->Material_Number();

		/*
		//Material Instance loading
		for (unsigned int flatbuffer_material_index = 0; flatbuffer_material_index < Material_Number; flatbuffer_material_index++) {
			assert(true || "Define Material Instance loading process!");
		}*/

		//We create Static Model file with 0 size Vector, so we should expand its size!
		MESHes.grow(RESOURCE->MESHes()->size());

		//For each mesh!
		for (unsigned int mesh_index = 0; mesh_index < RESOURCE->MESHes()->Length(); mesh_index++) {
			auto MESH = RESOURCE->MESHes()->Get(mesh_index);
			if (MESH == nullptr) {
				TuranAPI::LOG_ERROR("Mesh isn't loaded because data was nullptr!");
				continue;
			}
			Static_Mesh_Data* Static_Mesh = new Static_Mesh_Data(MESH->AttributeLayout()->size());
			MESHes.push_back(Static_Mesh);
			Static_Mesh->Material_Index = MESH->Material_Index();
			Static_Mesh->DATA_SIZE = MESH->DATA()->size();
			Static_Mesh->DATA = Allocator->Allocate_MemoryBlock(Static_Mesh->DATA_SIZE);
			memcpy(Static_Mesh->DATA, MESH->DATA()->Data(), Static_Mesh->DATA_SIZE);
			GetLayout_FromFlatbuffer(MESH->AttributeLayout(), Static_Mesh->DataLayout);
			Static_Mesh->VERTEX_NUMBER = MESH->VertexNumber();
		}

		std::string strNAME = path;
		strNAME = strNAME.substr(strNAME.find_last_of('/') + 1);
		strNAME = strNAME.substr(0, strNAME.find_last_of('.'));

		//Finalization
		ID = id;
		NAME = strNAME.c_str();
		PATH = path;
	}


	void Static_Model_Data::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		//Create a flatbufferbuilder and FileList to build the data!
		flatbuffers::FlatBufferBuilder builder(1024);

		Vector<flatbuffers::Offset<EditorAsset::MESH>> FB_MESHes(LASTUSEDALLOCATOR, 0, MESHes.size());
		//For each mesh of the model!
		for (unsigned int mesh_index = 0; mesh_index < MESHes.size(); mesh_index++) {
			Static_Mesh_Data* MESH = MESHes[mesh_index];

			auto FBMESH_DATA = builder.CreateVector((int8_t*)MESH->DATA, MESH->DATA_SIZE);
			auto FBMESH_ATTRIBUTE = ConvertLayout_toFlatbuffer(MESH->DataLayout, builder);

			auto FINISHED_MESH_DATA = EditorAsset::CreateMESH(builder, FBMESH_DATA, FBMESH_ATTRIBUTE, MESH->Material_Index, MESH->VERTEX_NUMBER);
			FB_MESHes.push_back(FINISHED_MESH_DATA);
		}
		auto FINISHED_MESHES_VECTOR = builder.CreateVector(FB_MESHes.data(), FB_MESHes.size());

		
		auto FINISHED_MODEL_DATA = EditorAsset::CreateSTATIC_MODEL(builder, FINISHED_MESHES_VECTOR, Material_Number);
		auto FINISHED_RESOURCE_DATA = EditorAsset::CreateResource(builder, EditorAsset::Resource_Type_Static_Model, FINISHED_MODEL_DATA.Union());
		builder.Finish(FINISHED_RESOURCE_DATA);

		void* data_ptr = builder.GetBufferPointer();
		unsigned int data_size = builder.GetSize();


		//Check if the data is complete!
		flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
		if (!EditorAsset::VerifyResourceBuffer(verifier)) {
			String Error_Message = "Data isn't verified for name: ";
			Error_Message.append(NAME);
			TuranAPI::LOG_CRASHING(Error_Message);
		}

		std::cout << "Exporting resource as a .meshcont: " << PATH << std::endl;
		TAPIFILESYSTEM::Write_BinaryFile(PATH, data_ptr, data_size);
	}
	Vector<unsigned int> Static_Model_Data::Upload_toGPU() {
		Vector<unsigned int> MeshBuffer_IDs(LASTUSEDALLOCATOR, 0, MESHes.size());
		for (unsigned int i = 0; i < MESHes.size(); i++) {
			const Static_Mesh_Data* MESH = MESHes[i];
			unsigned int MESHBUFFER_ID = GFXContentManager->Upload_MeshBuffer(MESH->DataLayout, MESH->DATA, MESH->DATA_SIZE, MESH->VERTEX_NUMBER);
			MeshBuffer_IDs.push_back(MESHBUFFER_ID);
		}
		return MeshBuffer_IDs;
	}

	void GetLayout_FromFlatbuffer(const flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>* Attributes, GFX_API::VertexAttributeLayout& Layout) {
		for (unsigned int i = 0; i < Attributes->size(); i++) {
			GFX_API::VertexAttribute* Attribute = new GFX_API::VertexAttribute;
			Layout.Attributes.push_back(Attribute);
			auto FBAttribute = Attributes->Get(i);


			Attribute->AttributeName = FBAttribute->AttributeName()->c_str();
			Attribute->Index = FBAttribute->AttributeIndex();
			Attribute->Stride = FBAttribute->Stride();
			Attribute->Start_Offset = FBAttribute->Start_Offset();
			Attribute->DATATYPE = GetDataType_FromFlatbuffer(FBAttribute->DataType());
		}
		Layout.Calculate_SizeperVertex();

		if (!Layout.VerifyAttributeLayout()) {
			TuranAPI::LOG_CRASHING("Imported Vertex Attribute isn't verified!");
		}
	}
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>> ConvertLayout_toFlatbuffer(const GFX_API::VertexAttributeLayout& layout, flatbuffers::FlatBufferBuilder& builder) {
		Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>> Attributes(LASTUSEDALLOCATOR, 0, layout.Attributes.size());
		for (unsigned char i = 0; i < layout.Attributes.size(); i++) {
			GFX_API::VertexAttribute* Attribute = layout.Attributes[i];
			Attributes.push_back(
				EditorAsset::CreateVERTEX_ATTRIBUTE(builder, builder.CreateString(Attribute->AttributeName),
					ConvertDataType_ToFlatbuffer(Attribute->DATATYPE), Attribute->Index, Attribute->Stride, Attribute->Start_Offset)
			);
		}
		return builder.CreateVector(Attributes.data(), Attributes.size());
	}

	GFX_API::UNIFORMTYPE GetDataType_FromFlatbuffer(EditorAsset::ATTRIBUTE_DATATYPE datatype) {
		switch (datatype) {
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_UBYTE8:
			return GFX_API::UNIFORMTYPE::VAR_UBYTE8;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_BYTE8:
			return GFX_API::UNIFORMTYPE::VAR_BYTE8;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_UINT32:
			return GFX_API::UNIFORMTYPE::VAR_UINT32;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_INT32:
			return GFX_API::UNIFORMTYPE::VAR_INT32;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_FLOAT32:
			return GFX_API::UNIFORMTYPE::VAR_FLOAT32;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC2:
			return GFX_API::UNIFORMTYPE::VAR_VEC2;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC3:
			return GFX_API::UNIFORMTYPE::VAR_VEC3;
		case EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC4:
			return GFX_API::UNIFORMTYPE::VAR_VEC4;
		default:
			TuranAPI::LOG_CRASHING("Unsupported convertion from Flatbuffer to GFX_API::UNIFORMTYPE!");
		}
	}
	EditorAsset::ATTRIBUTE_DATATYPE ConvertDataType_ToFlatbuffer(GFX_API::UNIFORMTYPE datatype) {
		switch (datatype) {
		case GFX_API::UNIFORMTYPE::VAR_UBYTE8:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_UBYTE8;
		case GFX_API::UNIFORMTYPE::VAR_BYTE8:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_BYTE8;
		case GFX_API::UNIFORMTYPE::VAR_UINT32:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_UINT32;
		case GFX_API::UNIFORMTYPE::VAR_INT32:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_INT32;
		case GFX_API::UNIFORMTYPE::VAR_FLOAT32:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_FLOAT32;
		case GFX_API::UNIFORMTYPE::VAR_VEC2:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC2;
		case GFX_API::UNIFORMTYPE::VAR_VEC3:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC3;
		case GFX_API::UNIFORMTYPE::VAR_VEC4:
			return EditorAsset::ATTRIBUTE_DATATYPE::ATTRIBUTE_DATATYPE_VEC4;
		default:
			TuranAPI::LOG_CRASHING("Unsupported convertion to Flatbuffer from GFX_API::UNIFORMTYPE!");
		}
	}
}