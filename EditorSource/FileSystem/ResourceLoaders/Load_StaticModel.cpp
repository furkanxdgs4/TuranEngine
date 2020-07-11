#include "EditorSource/Editor_Includes.h"
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"


namespace TuranEditor {
	//HELPER FUNCTIONs
	void GetLayout_FromFlatbuffer(const flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>* Attributes, GFX_API::VertexAttributeLayout& Layout);
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>>> ConvertLayout_toFlatbuffer(const GFX_API::VertexAttributeLayout& layout, flatbuffers::FlatBufferBuilder& builder);
	GFX_API::UNIFORMTYPE GetDataType_FromFlatbuffer(EditorAsset::ATTRIBUTE_DATATYPE datatype);
	EditorAsset::ATTRIBUTE_DATATYPE ConvertDataType_ToFlatbuffer(GFX_API::UNIFORMTYPE datatype);



	bool Verify_StaticModel(Resource_Identifier* IDENTIFIER) {
		Static_Model* RESOURCE = (Static_Model*)IDENTIFIER->DATA;
		if (!RESOURCE) {
			EDITOR_FILESYSTEM->Load_Resource(IDENTIFIER->ID);
		}
		if (RESOURCE->MESHes.size() > 0) {
			for (unsigned int mesh_index = 0; mesh_index < RESOURCE->MESHes.size(); mesh_index++) {
				Static_Mesh_Data* MESH = RESOURCE->MESHes[mesh_index];
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
	void* Load_StaticModel(Resource_Identifier* IDENTIFIER) {
		std::cout << "Loading a model, ID is: " << IDENTIFIER->ID << std::endl;
		unsigned int data_size = 0;
		void* file_data = TAPIFILESYSTEM::Read_BinaryFile(IDENTIFIER->PATH.c_str(), data_size, LASTUSEDALLOCATOR);
		if (!file_data) {
			TuranAPI::LOG_ERROR("Loading failed! Static Model's file isn't found!");
			return nullptr;
		}
		auto RESOURCE_typeless = EditorAsset::GetResource(file_data);
		if (RESOURCE_typeless == nullptr) {
			TuranAPI::LOG_ERROR("Loading failed! Model isn't a valid resource!");
			return nullptr;
		}
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Static_Model();

		if (!RESOURCE) {
			TuranAPI::LOG_ERROR("Type isn't Static Model, Type definition is wrong!");
			return nullptr;
		}

		Static_Model* MODEL = new Static_Model;
		IDENTIFIER->DATA = MODEL;

		//Get how much material the model has!
		MODEL->Material_Number = RESOURCE->Material_Number();

		/*
		//Material Instance loading
		for (unsigned int flatbuffer_material_index = 0; flatbuffer_material_index < Material_Number; flatbuffer_material_index++) {
			assert(true || "Define Material Instance loading process!");
		}*/


		//For each mesh!
		for (unsigned int mesh_index = 0; mesh_index < RESOURCE->MESHes()->Length(); mesh_index++) {
			auto MESH = RESOURCE->MESHes()->Get(mesh_index);
			if (MESH == nullptr) {
				TuranAPI::LOG_ERROR("Mesh isn't loaded because data was nullptr!");
				continue;
			}
			Static_Mesh_Data* Static_Mesh = new Static_Mesh_Data(MESH->AttributeLayout()->size());
			MODEL->MESHes.push_back(Static_Mesh);
			Static_Mesh->Material_Index = MESH->Material_Index();
			Static_Mesh->DATA_SIZE = MESH->DATA()->size();
			Static_Mesh->DATA = LASTUSEDALLOCATOR->Allocate_MemoryBlock(Static_Mesh->DATA_SIZE);
			memcpy(Static_Mesh->DATA, MESH->DATA()->Data(), Static_Mesh->DATA_SIZE);
			GetLayout_FromFlatbuffer(MESH->AttributeLayout(), Static_Mesh->DataLayout);
			Static_Mesh->VERTEX_NUMBER = MESH->VertexNumber();
		}

		return MODEL;
	}
	void Save_StaticModel(Resource_Identifier* IDENTIFIER) {
		Static_Model* MODEL = (Static_Model*)IDENTIFIER->DATA;

		//Create a flatbufferbuilder and FileList to build the data!
		flatbuffers::FlatBufferBuilder builder(1024);

		vector<flatbuffers::Offset<EditorAsset::MESH>> FB_MESHes;
		//For each mesh of the model!
		for (unsigned int mesh_index = 0; mesh_index < MODEL->MESHes.size(); mesh_index++) {
			Static_Mesh_Data* MESH = MODEL->MESHes[mesh_index];

			auto FBMESH_DATA = builder.CreateVector((int8_t*)MESH->DATA, MESH->DATA_SIZE);
			auto FBMESH_ATTRIBUTE = ConvertLayout_toFlatbuffer(MESH->DataLayout, builder);

			auto FINISHED_MESH_DATA = EditorAsset::CreateMESH(builder, FBMESH_DATA, FBMESH_ATTRIBUTE, MESH->Material_Index, MESH->VERTEX_NUMBER);
			FB_MESHes.push_back(FINISHED_MESH_DATA);
		}
		auto FINISHED_MESHES_vector = builder.CreateVector(FB_MESHes.data(), FB_MESHes.size());


		auto FINISHED_MODEL_DATA = EditorAsset::CreateSTATIC_MODEL(builder, FINISHED_MESHES_vector, MODEL->Material_Number);
		auto FINISHED_RESOURCE_DATA = EditorAsset::CreateResource(builder, EditorAsset::Resource_Type_Static_Model, FINISHED_MODEL_DATA.Union());
		builder.Finish(FINISHED_RESOURCE_DATA);

		void* data_ptr = builder.GetBufferPointer();
		unsigned int data_size = builder.GetSize();


		//Check if the data is complete!
		flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
		if (!EditorAsset::VerifyResourceBuffer(verifier)) {
			string Error_Message = "Data isn't verified for name: ";
			Error_Message.append(IDENTIFIER->Get_Name());
			TuranAPI::LOG_CRASHING(Error_Message);
		}

		std::cout << "Exporting resource as a .meshcont: " << IDENTIFIER->PATH << std::endl;
		TAPIFILESYSTEM::Write_BinaryFile(IDENTIFIER->PATH.c_str(), data_ptr, data_size);
	}




	//HELPER FUNCTIONs

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
		vector<flatbuffers::Offset<EditorAsset::VERTEX_ATTRIBUTE>> Attributes;
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