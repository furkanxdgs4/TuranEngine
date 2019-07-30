#pragma once
#include "Includes.h"

#include "Mesh_Data_Class.h"
#include "Static_Model_Class.h"
#include "MESH_OBJs.h"

class Mesh_Loader {
private:
	static void Load_All_Static_Meshes();
	static void Process_Node(const aiScene* Scene, aiNode* Node, vector<vector<unsigned int>>* mesh_ptr);
	static void Create_Meshes(const aiScene* Scene, const vector<vector<unsigned int>>* MeshList_per_Material, Static_Model* Imported_Model, string name);
public:

	static void Load_All_Meshes();
	Mesh_Loader() {
		Load_All_Meshes();
	}
};

Mesh_Loader MESH_LOADER_OBJ;

void Mesh_Loader::Load_All_Meshes() {
	Load_All_Static_Meshes();
}
/*This functions loads models as merges every mesh that has same material index in .obj!
So, we have to detect meshes that has same material index and create one mesh from them!
Loading Scheme:
1) First, create a vector to store all created Static Meshes! When a model loaded, created Static_Mesh*s are put into this vector!
2) For each Mesh_File, do the things below!
3) Read the file!
4) Create a vector to store mesh indexes that has same material for every material of the model!
5) Process every node in the model to put mesh indexes into proper meshlist defined above!
6) When proccessing done, call Create_Meshes() and create one mesh for each material index!
7) When Create_Meshes() finished, put created Static_Mesh*s into ALL_Static_Meshes
*/
void Mesh_Loader::Load_All_Static_Meshes() {
	vector<Static_Model*> ALL_Static_Models;
	for (Mesh_File* mesh_file : Mesh_File::Get_All_OBJs()) {
		double time = glfwGetTime();
		Assimp::Importer import;
		const aiScene* Scene = import.ReadFile(mesh_file->PATH, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace
			| aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData | aiProcess_RemoveRedundantMaterials);

		//Store mesh parts in a Model!
		Static_Model* Loaded_Model = new Static_Model;

		//Check if scene reading errors!
		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
			cout << "Failed on Loading Mesh with Assimp; " << import.GetErrorString() << endl;
			return;
		}

		//Store Assimp mesh indexes per material to access and store vertex attributes later in Create_Meshes();
		vector<vector<unsigned int>> MeshList_per_Material;
		for (unsigned int i = 0; i < Scene->mNumMaterials; i++) {
			MeshList_per_Material.push_back(vector<unsigned int>());
		}

		//Set per material mesh_list vector!
		Process_Node(Scene, Scene->mRootNode, &MeshList_per_Material);

		Create_Meshes(Scene, &MeshList_per_Material, Loaded_Model, mesh_file->NAME);

		//Add imported mesh to ALL_Static_Meshes vector!
		ALL_Static_Models.push_back(Loaded_Model);
	}
	Static_Model::ALL_MODELs = ALL_Static_Models;
}


void Mesh_Loader::Process_Node(const aiScene* Scene, aiNode* Node, vector<vector<unsigned int>>* mesh_indexes_per_material) {
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
}

void Mesh_Loader::Create_Meshes(const aiScene* Scene, const vector<vector<unsigned int>>* MeshList_per_Material, Static_Model* Imported_Model, string name) {
	//For each material index in model, create a StaticMesh and fill with data!
	for (unsigned int meshlist_index = 0; meshlist_index < MeshList_per_Material->size(); meshlist_index++) {
		vector<unsigned int> MeshList = (*MeshList_per_Material)[meshlist_index];
		Mesh_Data* mesh = new Mesh_Data;

		//Get total number of vertices and indices to allocate memory!
		unsigned int mesh_total_numvertices = 0;
		unsigned int mesh_total_numindices = 0;
		for (unsigned int assimp_mesh_index : MeshList) {
			aiMesh* assimp_mesh = Scene->mMeshes[assimp_mesh_index];
			mesh_total_numvertices += assimp_mesh->mNumVertices;
			mesh_total_numindices += assimp_mesh->mNumFaces * 3;
		}
		
		mesh->VERTEX_NUMBER = mesh_total_numvertices;
		mesh->INDICEs_LENGTH = mesh_total_numindices;

		vec3* Positions_Array = new vec3[mesh_total_numvertices];
		vec2* TextCoords_Array = new vec2[mesh_total_numvertices];
		vec3* Normals_Array = new vec3[mesh_total_numvertices];
		vec3* Tangents_Array = new vec3[mesh_total_numvertices];
		vec3* Bitangents_Array = new vec3[mesh_total_numvertices];
		unsigned int* Indices_Array = new unsigned int[mesh_total_numindices];
		
		mesh->POSITIONs = Positions_Array;
		mesh->TEXTCOORDs = TextCoords_Array;
		mesh->NORMALs = Normals_Array;
		mesh->TANGENTs = Tangents_Array;
		mesh->BITANGENTs = Bitangents_Array;
		mesh->INDICEs = Indices_Array;

		//For each created mesh, name meshes as "(Mesh_object name)0", "(Mesh_object name)1" etc.!
		mesh->NAME = name + to_string(meshlist_index);

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

		Imported_Model->NAME = name;
		Imported_Model->Meshes_of_Model.push_back(mesh);
	}
}