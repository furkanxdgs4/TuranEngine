#include "Model_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "TuranAPI/IMGUI/IMGUI_Core.h"

#include "EditorSource/Editors/Status_Window.h"

//Assimp libraries to load Model
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace TuranAPI::File_System;
using namespace TuranAPI::IMGUI;

Model_Import_Window::Model_Import_Window(TuranAPI::File_System::FileSystem* filesystem) : IMGUI_WINDOW("Model Import"), FILESYSTEM(filesystem) {}

void Model_Import_Window::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
	}

	IMGUI::Input_Text("File Path", &MODEL_IMPORT_PATH);
	IMGUI::Input_Text("Output Folder", &OUTPUT_FOLDER);
	IMGUI::Input_Text("Output Name", &OUTPUT_NAME);
	string PATH = OUTPUT_FOLDER + OUTPUT_NAME;
	//If Input is written!
	if (IMGUI::Button("Import")) {
		string status;

		//Check if this resource is already loaded to Content_List!
		for (Resource_Type* RESOURCE : *FILESYSTEM->Get_Const_FileListContentVector()) {
			if (OUTPUT_NAME == RESOURCE->NAME) {
				status = "Resource is already loaded and is in the Resource List!";
				Status_Window* error_window = new Status_Window(status);
				return;
			}
		}
		//If application arrives here, Resource isn't loaded to Content_List before!

		Resource_Type* imported_resource = Model_Loader::Import_Model(MODEL_IMPORT_PATH, &PATH, &status);
		Status_Window* error_window = new Status_Window(status);
		if (imported_resource) {
			FILESYSTEM->Add_Content_toFileList(imported_resource);
		}
	}

	IMGUI::End_Window();
}





/*This functions loads models as merges every mesh that has same material index in .obj!
So, we have to detect meshes that has same material index and create one mesh from them!
Loading Scheme:
1) Read the file!
2) Create a vector to store mesh indexes that has same material for every material of the model!
3) Process every node in the model to put mesh indexes into proper meshlist defined above!
4) When proccessing done, call Create_Meshes() and create one mesh for each material index!
5) When Create_Meshes() finished, put created Static_Mesh*s into ALL_Static_Meshes
*/

/*
Static_Model_Data* Load_Model(const aiScene* Scene, const vector<vector<unsigned int>>* MeshList_per_Material, string name, string Directory) {
	unsigned int MESH_NUMBER = MeshList_per_Material->size();

	Static_Model_Data* model_data = new Static_Model_Data;
	Static_Mesh_Data** mesh_array = new Static_Mesh_Data * [MESH_NUMBER];

	//For each material index in model, create a StaticMesh and fill with data!
	for (unsigned int meshlist_index = 0; meshlist_index < MESH_NUMBER; meshlist_index++) {
		vector<unsigned int> MeshList = (*MeshList_per_Material)[meshlist_index];
		Static_Mesh_Data* mesh_data = new Static_Mesh_Data;

		//Get total number of vertices and indices to allocate memory!
		unsigned int mesh_total_numvertices = 0;
		unsigned int mesh_total_numindices = 0;
		for (unsigned int assimp_mesh_index : MeshList) {
			aiMesh* assimp_mesh = Scene->mMeshes[assimp_mesh_index];
			mesh_total_numvertices += assimp_mesh->mNumVertices;
			mesh_total_numindices += assimp_mesh->mNumFaces * 3;
		}

		mesh_data->VERTEX_NUMBER = mesh_total_numvertices;
		mesh_data->INDICEs_LENGTH = mesh_total_numindices;

		vec3* Positions_Array = new vec3[mesh_total_numvertices];
		vec2* TextCoords_Array = new vec2[mesh_total_numvertices];
		vec3* Normals_Array = new vec3[mesh_total_numvertices];
		vec3* Tangents_Array = new vec3[mesh_total_numvertices];
		vec3* Bitangents_Array = new vec3[mesh_total_numvertices];
		unsigned int* Indices_Array = new unsigned int[mesh_total_numindices];

		mesh_data->POSITIONs = Positions_Array;
		mesh_data->TEXTCOORDs = TextCoords_Array;
		mesh_data->NORMALs = Normals_Array;
		mesh_data->TANGENTs = Tangents_Array;
		mesh_data->BITANGENTs = Bitangents_Array;
		mesh_data->INDICEs = Indices_Array;

		//Get mesh datas from assimp meshes!
		for (unsigned int meshlist_meshindex = 0; meshlist_meshindex < MeshList.size(); meshlist_meshindex++) {

			//Get array offset for this assimp mesh!
			unsigned int vertex_offset = 0;
			unsigned int indice_offset = 0;
			for (unsigned int i = 0; i < meshlist_meshindex; i++) {
				unsigned int assimp_mesh_index = MeshList[i];
				aiMesh* assimp_mesh = Scene->mMeshes[assimp_mesh_index];

				vertex_offset += assimp_mesh->mNumVertices;
				indice_offset += assimp_mesh->mNumFaces * 3;
			}

			aiMesh* assimp_mesh = Scene->mMeshes[MeshList[meshlist_meshindex]];

			//Set Positions!
			for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
				Positions_Array[vertex_offset + i].x = assimp_mesh->mVertices[i].x;
				Positions_Array[vertex_offset + i].y = assimp_mesh->mVertices[i].y;
				Positions_Array[vertex_offset + i].z = assimp_mesh->mVertices[i].z;
			}
			//Set TextCoords!
			for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
				TextCoords_Array[vertex_offset + i].x = assimp_mesh->mTextureCoords[0][i].x;
				TextCoords_Array[vertex_offset + i].y = assimp_mesh->mTextureCoords[0][i].y;
			}
			//Set Normals!
			for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
				Normals_Array[vertex_offset + i].x = assimp_mesh->mNormals[i].x;
				Normals_Array[vertex_offset + i].y = assimp_mesh->mNormals[i].y;
				Normals_Array[vertex_offset + i].z = assimp_mesh->mNormals[i].z;
			}
			//Set Tangents!
			for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
				Tangents_Array[vertex_offset + i].x = assimp_mesh->mTangents[i].x;
				Tangents_Array[vertex_offset + i].y = assimp_mesh->mTangents[i].y;
				Tangents_Array[vertex_offset + i].z = assimp_mesh->mTangents[i].z;
			}
			//Set Bitangents!
			for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++) {
				Bitangents_Array[vertex_offset + i].x = assimp_mesh->mBitangents[i].x;
				Bitangents_Array[vertex_offset + i].y = assimp_mesh->mBitangents[i].y;
				Bitangents_Array[vertex_offset + i].z = assimp_mesh->mBitangents[i].z;
			}
			//Set Indices!
			for (unsigned int face = 0; face < assimp_mesh->mNumFaces; face++) {
				aiFace assimp_face = assimp_mesh->mFaces[face];
				//We are loading meshes as triangulated, so each face has 3 indices!
				for (unsigned int indice = 0; indice < assimp_face.mNumIndices; indice++) {
					Indices_Array[indice_offset + (face * 3) + indice] = assimp_face.mIndices[indice];
				}
			}
		}
		/*
		//Upload Textures, Create a Material and Set it to Mesh!
		GFX_Material* material = new Surface_Material;
		mesh->MATERIAL = material;

		aiMaterial* assimp_material = Scene->mMaterials[meshlist_index];

		//If there is any diffuse texture!
		if (assimp_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString file_name;
			assimp_material->GetTexture(aiTextureType_DIFFUSE, 0, &file_name);


			const char* path = (Directory + '/' + (string)file_name.C_Str()).c_str();
			int width, height, number_of_channels;
			stbi_set_flip_vertically_on_load(false);
			unsigned char* data = stbi_load(path, &width, &height, &number_of_channels, 0);
			if (!data)
			{
				std::cout << "Failed to load texture" << std::endl;
				return;
			}


			GFX_Texture* texture = new GFX_Texture;
			switch (number_of_channels) {
			case 3:
				texture->CHANNEL_TYPE = GFX_TEXTURE_RGB;
				break;
			case 4:
				texture->CHANNEL_TYPE = GFX_TEXTURE_RGBA;
			}
			texture->DIMENSION = GFX_TEXTURE_2D;
			texture->FORMAT_VALUETYPE = GFX_UNSIGNED_BYTE;
			texture->MIPMAP_FILTERING = GFX_LINEAR_FROM_1MIP;
			texture->WRAPPING = GFX_REPEAT;
			texture->PATH = path;
			texture->DATA = data;
			texture->WIDTH = width;
			texture->HEIGHT = height;
			material->Set_Uniform_Data("texture_diffuse", (void*)& texture->ID);
		}
		
		mesh_array[meshlist_index] = mesh_data;
	}

	for (unsigned int i = 0; i < MESH_NUMBER; i++) {
		Static_Mesh_Data* mesh = mesh_array[i];

		unsigned int total_size = 0;
		total_size += mesh->VERTEX_NUMBER * ((sizeof(vec3) * 4) + sizeof(vec2));
		total_size += mesh->INDICEs_LENGTH * sizeof(unsigned int);
		cout << "Size in bytes: " << total_size << endl;
	}

	model_data->Set_Mesh_Data(mesh_array, MESH_NUMBER);
	return model_data;
}



void Process_Node(const aiScene* Scene, aiNode* Node, vector<vector<unsigned int>>* mesh_indexes_per_material) {
	for (unsigned int i = 0; i < (Node->mNumMeshes); i++) {
		//Find mesh's material index and add mesh to its material's vector!
		unsigned int Assimp_mesh_index = Node->mMeshes[i];
		aiMesh* mesh = Scene->mMeshes[Assimp_mesh_index];
		unsigned int material_index = mesh->mMaterialIndex;
		(*mesh_indexes_per_material)[material_index].push_back(Assimp_mesh_index);
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		//Process child nodes!
		Process_Node(Scene, Node->mChildren[i], mesh_indexes_per_material);
	}
}*/

Static_Mesh_Data* Load_MeshData(const aiMesh* data) {
	Static_Mesh_Data* MESH = new Static_Mesh_Data;
	MESH->Material_Index = data->mMaterialIndex;

	//Vertex Positions
	vec3* Positions = new vec3[data->mNumVertices];
	for (unsigned int i = 0; i < data->mNumVertices; i++) {
		Positions[i].x = data->mVertices[i].x;
		Positions[i].y = data->mVertices[i].y;
		Positions[i].z = data->mVertices[i].z;
	}

	//Vertex Normals
	vec3* Normals = new vec3[data->mNumVertices];
	for (unsigned int i = 0; i < data->mNumVertices; i++) {
		Normals[i].x = data->mNormals[i].x;
		Normals[i].y = data->mNormals[i].y;
		Normals[i].z = data->mNormals[i].z;
	}

	//Tangents
	vec3* Tangents = new vec3[data->mNumVertices];
	for (unsigned int i = 0; i < data->mNumVertices; i++) {
		Tangents[i].x = data->mTangents[i].x;
		Tangents[i].y = data->mTangents[i].y;
		Tangents[i].z = data->mTangents[i].z;
	}

	//Bitangents
	vec3* Bitangents = new vec3[data->mNumVertices];
	for (unsigned int i = 0; i < data->mNumVertices; i++) {
		Bitangents[i].x = data->mBitangents[i].x;
		Bitangents[i].y = data->mBitangents[i].y;
		Bitangents[i].z = data->mBitangents[i].z;
	}

	//Texture Coordinates
	if (data->HasTextureCoords(0)) {
		vec2* TextCoords = new vec2[data->mNumVertices];
		for (unsigned int i = 0; i < data->mNumVertices; i++) {
			TextCoords[i].x = data->mTextureCoords[0][i].x;
			TextCoords[i].y = data->mTextureCoords[0][i].y;
		}
		MESH->TEXTCOORDs = TextCoords;
	}
	else {
		MESH->TEXTCOORDs = nullptr;
	}
	unsigned int* Indices = new unsigned int[data->mNumFaces * 3];
	for (unsigned int i = 0; i < data->mNumFaces; i++) {
		aiFace* FACE = &(data->mFaces[i]);
		for (unsigned int t = 0; t < 3; t++) {
			Indices[(i * 3) + t] = FACE->mIndices[t];
		}
	}
	MESH->VERTEX_NUMBER = data->mNumVertices;
	MESH->INDICEs_LENGTH = data->mNumFaces * 3;
	MESH->POSITIONs = Positions;
	MESH->NORMALs = Normals;
	MESH->TANGENTs = Tangents;
	MESH->BITANGENTs = Bitangents;
	MESH->INDICEs = Indices;

	return MESH;
}

//Loads the mesh parts to the vector!
void Process_Node(const aiScene* Scene, aiNode* Node, vector<Static_Mesh_Data*>* Meshes) {
	for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
		unsigned int Assimp_mesh_index = Node->mMeshes[i];
		aiMesh* mesh = Scene->mMeshes[Assimp_mesh_index];
		auto Mesh_Data = Load_MeshData(mesh);
		Meshes->push_back(Mesh_Data);
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		//Process child nodes!
		Process_Node(Scene, Node->mChildren[i], Meshes);
	}
}

Static_Model_Data* MergeMeshes_toModel(const aiScene* Scene, Static_Mesh_Data** MESH_DATAs, unsigned int MESH_NUMBER, string NAME, string DIRECTORY) {
	Static_Model_Data* MODEL = new Static_Model_Data;
	MODEL->NAME = NAME;
	MODEL->PATH = DIRECTORY;

	MODEL->MESH_ARRAY_PTR = MESH_DATAs;
	MODEL->MESH_NUMBER = MESH_NUMBER;

	return MODEL;
}


//Loads a model from a understandable format (OBJ, FBX...) for Asset Importer and Verifies Data.
TuranAPI::File_System::Resource_Type* Model_Loader::Import_Model(const string& path, const string* output_path, string* compilation_status) {
	Assimp::Importer import;
	const aiScene* Scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace
		| aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData | aiProcess_RemoveRedundantMaterials
	);

	//Check if scene reading errors!
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
		*compilation_status = "Failed on Loading Mesh with Assimp; " + (string)import.GetErrorString();
		return nullptr;
	}


	vector<Static_Mesh_Data*> MESH_DATAs;
	Process_Node(Scene, Scene->mRootNode, &MESH_DATAs);

	//Convert vector to array!
	Static_Mesh_Data** MESHes = new Static_Mesh_Data*[MESH_DATAs.size()];
	for (unsigned int i = 0; i < MESH_DATAs.size(); i++) {
		MESHes[i] = MESH_DATAs[i];
	}

	//Include resource data format too! (name.obj, name.fbx etc.)
	string PATH = *output_path + ".meshcont";

	//Find directory to load textures!
	string Directory = path.substr(0, path.find_last_of('/'));

	string NAME = output_path->substr(output_path->find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	//Store mesh parts in a Model!
	//Note: Now just merge all of the mesh parts in a model, no load operations!
	Static_Model_Data* Loaded_Model = MergeMeshes_toModel(Scene, MESHes, MESH_DATAs.size(), NAME, Directory);

	//Finalization
	cout << "Compiled the model name: " << NAME << endl;
	Loaded_Model->NAME = NAME;
	Loaded_Model->PATH = PATH;


	if (Loaded_Model->Verify_Resource_Data()) {
		//ID generation is handled while writing to a file!
		FileSystem::Write_a_Resource_toDisk(Loaded_Model);

		*compilation_status = "Successfully compiled and saved to disk!";
		return Loaded_Model;
	}
	else {
		*compilation_status = "Loaded model data isn't verified successfully!";
		return nullptr;
	}
}
