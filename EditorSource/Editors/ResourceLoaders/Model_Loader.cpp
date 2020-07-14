#include "Model_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

#include "EditorSource/Editors/Status_Window.h"

//Assimp libraries to load Model
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"

using namespace TuranAPI;

namespace TuranEditor {
	Model_Import_Window::Model_Import_Window() : IMGUI_WINDOW("Model Import") {
		IMGUI_REGISTERWINDOW(this);
	}

	Model_Import_Window::~Model_Import_Window() {
	}

	void Model_Import_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
			IMGUI->End_Window();
		}

		IMGUI->Input_Text("File Path", &MODEL_IMPORT_PATH);
		IMGUI->Input_Text("Output Folder", &OUTPUT_FOLDER);
		IMGUI->Input_Text("Output Name", &OUTPUT_NAME);
		//If Input is written!
		if (IMGUI->Button("Import")) {
			string status;

			//Check if this resource is already loaded to Content_List!
			for (unsigned int i = 0; i < TuranEditor::EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
				Resource_Identifier* RESOURCE = TuranEditor::EDITOR_FILESYSTEM->Get_AssetList()[i];
				if (OUTPUT_NAME == RESOURCE->Get_Name()) {
					status = "Resource is already loaded and is in the Resource List!";
					TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());
					return;
				}
			}

			string PATH = OUTPUT_FOLDER;
			PATH.append(OUTPUT_NAME);
			PATH.append(".meshcont");
			Static_Model* imported_resource = Model_Loader::Import_Model(MODEL_IMPORT_PATH.c_str(), &status);
			TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());
			if (imported_resource) {
				Resource_Identifier* RESOURCE = new Resource_Identifier;
				RESOURCE->PATH = PATH;
				RESOURCE->DATA = imported_resource;
				RESOURCE->TYPE = RESOURCETYPEs::EDITOR_STATICMODEL;
				TuranEditor::EDITOR_FILESYSTEM->Add_anAsset_toFileList(RESOURCE);
				//Add_anAsset_toFileList gave an ID to Resource, so we can save the resource now!
				EDITOR_FILESYSTEM->Save_Resource(RESOURCE->ID);
			}
		}
		IMGUI->End_Window();
	}


	struct Attribute_BitMask {
		bool TextCoords : 2;
		bool VertexColors : 2;
		bool Normal : 1;
		bool Tangent : 1;
		bool Bitangent : 1;
		bool Bones : 1;
		Attribute_BitMask() {
			TextCoords = 0;
			VertexColors = 0;
			Normal = false;
			Tangent = false;
			Bitangent = false;
			Bones = false;
		}
	};

	void Find_AvailableAttributes(aiMesh* data, Attribute_BitMask& Available_Attributes) {
		if (data->HasNormals()) {
			Available_Attributes.Normal = true;
		}
		else {
			Available_Attributes.Normal = false;
		}
		if (data->HasTangentsAndBitangents()) {
			Available_Attributes.Bitangent = true;
			Available_Attributes.Tangent = true;
		}
		else {
			Available_Attributes.Bitangent = false;
			Available_Attributes.Tangent = false;
		}
		//Maximum Available UV Set Number is 4
		Available_Attributes.TextCoords = 0;
		for (unsigned int i = 0; i < 4; i++) {
			if (data->HasTextureCoords(i)) {
				Available_Attributes.TextCoords += 1;
			}
		}
		//Maximum Available VertexColor Set Number is 4
		Available_Attributes.VertexColors = 0;
		for (unsigned int i = 0; i < 4; i++) {
			if (data->HasVertexColors(i)) {
				Available_Attributes.VertexColors += 1;
			}
		}
		if (data->HasBones()) {
			Available_Attributes.Bones = true;
		}
		else {
			Available_Attributes.Bones = false;
		}
	}

	//Number of available attributes (except Bones, because there are lots of things hard for now!)
	unsigned int Find_NumberofAvailableAttributes(Attribute_BitMask& attributesinfo) {
		unsigned int number = 0;
		number += attributesinfo.VertexColors;
		number += attributesinfo.TextCoords;
		if (attributesinfo.Normal) {
			number += 1;
		}
		if (attributesinfo.Tangent) {
			number += 1;
		}
		if (attributesinfo.Bitangent) {
			number += 1;
		}
		return number;
	}

	void Load_MeshData(const aiMesh* data, Attribute_BitMask& Attribute_Info, Static_Model* Model) {
		//unsigned int numberofattributes = Find_NumberofAvailableAttributes(Attribute_Info);

		Static_Mesh_Data* MESH = new Static_Mesh_Data(1);
		Model->MESHes.push_back(MESH);

		//You should choose the Vertex Attribute Layout, then set the data according to it. So I should create a Attribute Layout Settings window here.
		//And give the user which attributes the imported asset has.
		MESH->Material_Index = data->mMaterialIndex;
		MESH->VERTEX_NUMBER = data->mNumVertices;

		/*Notes:
		1) Only Position attribute is loaded for now
		2) In summer, I will code an Editor to set Attribute Layout so the code below is a fixed way for now
		*/
		{
			GFX_API::VertexAttribute PositionAttribute;
			//Create Position Attribute!
			PositionAttribute.AttributeName = "Positions";
			PositionAttribute.Index = 0;
			PositionAttribute.Stride = 0;
			PositionAttribute.Start_Offset = 0;
			PositionAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC3;
			MESH->DataLayout.Attributes.push_back(PositionAttribute);

			//I don't want to try to support the other attributes for now but the code should be like that!
			/*
			//Use this index to set Attribute index for each attribute
			unsigned int NextAttribute_Index = 1;

			//Load UV Sets!
			for (unsigned int i = 0; i < Attribute_Info.TextCoords; i++) {
				GFX_API::VertexAttribute TextCoordAttribute;
				TextCoordAttribute.AttributeName.append("UV Set ");
				TextCoordAttribute.AttributeName.append(std::to_string(i).c_str());
				TextCoordAttribute.Index = NextAttribute_Index + i;
				TextCoordAttribute.Stride = 0;
				TextCoordAttribute.Start_Offset = 0;
				switch (data->mNumUVComponents[i]) {
				case 1:
					TextCoordAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_FLOAT32;
					break;
				case 2:
					TextCoordAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC2;
					break;
				case 3:
					TextCoordAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC3;
					break;
				default:
					TuranAPI::LOG_CRASHING("There is a error in Texture Coordinate!");
				}
				AttributeLayout.Attributes.push_back(TextCoordAttribute);
			}
			NextAttribute_Index += Attribute_Info.TextCoords;

			//Load Vertex Color Sets!
			for (unsigned int i = 0; i < Attribute_Info.VertexColors; i++) {
				GFX_API::VertexAttribute VertColorAttribute;
				VertColorAttribute.AttributeName.append("VertColor Set ");
				VertColorAttribute.AttributeName.append(std::to_string(i).c_str());
				VertColorAttribute.Index = NextAttribute_Index + i;
				VertColorAttribute.Stride = 0;
				VertColorAttribute.Start_Offset = 0;
				VertColorAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC4;
				AttributeLayout.Attributes.push_back(VertColorAttribute);
			}
			NextAttribute_Index += Attribute_Info.VertexColors;

			if (Attribute_Info.Normal) {
				GFX_API::VertexAttribute NormalAttribute;
				NormalAttribute.AttributeName = "Normals";
				NormalAttribute.Index = NextAttribute_Index;
				NextAttribute_Index++;
				NormalAttribute.Stride = 0;
				NormalAttribute.Start_Offset = 0;
				NormalAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC3;
				AttributeLayout.Attributes.push_back(NormalAttribute);
			}
			if (Attribute_Info.Tangent) {
				GFX_API::VertexAttribute TangentAttribute;
				TangentAttribute.AttributeName = "Tangents";
				TangentAttribute.Index = NextAttribute_Index;
				NextAttribute_Index++;
				TangentAttribute.Stride = 0;
				TangentAttribute.Start_Offset = 0;
				TangentAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC3;
				AttributeLayout.Attributes.push_back(TangentAttribute);
			}
			if (Attribute_Info.Bitangent) {
				GFX_API::VertexAttribute BitangentAttribute;
				BitangentAttribute.AttributeName = "Bitangents";
				BitangentAttribute.Index = NextAttribute_Index;
				NextAttribute_Index++;
				BitangentAttribute.Stride = 0;
				BitangentAttribute.Start_Offset = 0;
				BitangentAttribute.DATATYPE = GFX_API::UNIFORMTYPE::VAR_VEC3;
				AttributeLayout.Attributes.push_back(BitangentAttribute);
			}
			if (Attribute_Info.Bones) {
				TuranAPI::LOG_ERROR("Loader doesn't support to import Bone attribute of the meshes!");
			}

			*/

			//Final Checks on Attribute Layout
			if (!MESH->DataLayout.VerifyAttributeLayout()) {
				TuranAPI::LOG_ERROR("Attribute Layout isn't verified, there is a problem somewhere!");
				return;
			}
			MESH->DataLayout.Calculate_SizeperVertex();

		}

		MESH->DATA_SIZE = MESH->DataLayout.size_pervertex * MESH->VERTEX_NUMBER;
		MESH->DATA = LASTUSEDALLOCATOR->Allocate_MemoryBlock(MESH->DATA_SIZE);
		if (!MESH->DATA) {
			TuranAPI::LOG_CRASHING("Allocator failed to create a buffer for mesh's data!");
			return;
		}


		//Now I need to fill the MESH->DATA!
		{
			//First, fill the position buffer!
			memcpy(MESH->DATA, data->mVertices, MESH->VERTEX_NUMBER * sizeof(vec3));
		}
	}



	//Loads a model from a understandable format (OBJ, FBX...) for Asset Importer and Verifies Data.
	Static_Model* Model_Loader::Import_Model(const char* path, string* compilation_status) {
		Assimp::Importer import;
		const aiScene* Scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace
			| aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData | aiProcess_RemoveRedundantMaterials
		);

		//Check if scene reading errors!
		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
			compilation_status->append("Failed on Loading Mesh with Assimp; ");
			compilation_status->append(import.GetErrorString());
			return nullptr;
		}

		if (Scene->mNumMeshes == 0) {
			compilation_status->append("Failed because there is no mesh in loaded scene!");
			return nullptr;
		}

		//Store mesh parts in a Model!
		//Note: Now just merge all of the mesh parts in a model, no load operations!
		Static_Model* Loaded_Model = new Static_Model;


		vector<Attribute_BitMask> aiMesh_Attributes;
		aiMesh_Attributes.resize(Scene->mNumMeshes);
		for (unsigned int i = 0; i < Scene->mNumMeshes; i++) {
			Find_AvailableAttributes(Scene->mMeshes[i], aiMesh_Attributes[i]);
		}



		//WITH AVAILABLE ATTRIBUTE INFO, HERE I SHOULD WRITE THE EDITOR CODE TO MANIPULATE ATTRIBUTE LAYOUT!
		//For now, I just use the default attribute layout (Stride = 0, Start_Offset = 0 for all attributes)
		//So, I write the imported resource's attribute info to the Status Window!
		Attribute_BitMask* bitmask = nullptr;
		{
			compilation_status->append("Note: Only Positions attribute is loaded for now because of limited time! In summer: I will support to load all of the attributes as the user wants!");
			for (unsigned int i = 0; i < Scene->mNumMeshes; i++) {
				bitmask = &aiMesh_Attributes[i];
				compilation_status->append("Mesh Index: ");
				compilation_status->append(std::to_string(i).c_str());
				compilation_status->append(" Texture Coordinate Sets Number: ");
				compilation_status->append(std::to_string(bitmask->TextCoords).c_str());
				compilation_status->append(" Vertex Color Sets Number: ");
				compilation_status->append(std::to_string(bitmask->VertexColors).c_str());
				if (bitmask->Normal) {
					compilation_status->append(" Vertex Normals're found");
				}
				else {
					compilation_status->append(" Vertex Normals're not found");
				}
				if (bitmask->Tangent) {
					compilation_status->append(" Tangents're found");
				}
				else {
					compilation_status->append(" Tangents're not found");
				}
				if (bitmask->Bitangent) {
					compilation_status->append(" Bitangents're found");
				}
				else {
					compilation_status->append(" Bitangents're not found");
				}
				if (bitmask->Bones) {
					compilation_status->append(" Bones're found ");
				}
				else {
					compilation_status->append(" Bones're not found ");
				}
			}
		}


		for (unsigned int i = 0; i < Scene->mNumMeshes; i++) {
			Load_MeshData(Scene->mMeshes[i], aiMesh_Attributes[i], Loaded_Model);
		}



		//Finalization
		compilation_status->append("Compiled the model successfully!");

		Loaded_Model->Material_Number = Scene->mNumMaterials;
		return Loaded_Model;
	}
}