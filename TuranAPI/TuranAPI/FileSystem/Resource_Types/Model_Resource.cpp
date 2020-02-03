#include "Model_Resource.h"
using namespace TuranAPI::File_System;

vector<Static_Model_Data*> Static_Model_Data::ALL_MODEL_DATAs = vector<Static_Model_Data*>{};

Static_Mesh_Data::~Static_Mesh_Data() {
	delete[] POSITIONs;
	delete[] NORMALs;
	delete[] TEXTCOORDs;
	delete[] TANGENTs;
	delete[] BITANGENTs;
}

bool Static_Mesh_Data::Verify_Mesh_Data() {
	if (VERTEX_NUMBER <= 0 || POSITIONs == nullptr || NORMALs == nullptr || TANGENTs == nullptr || BITANGENTs == nullptr || TEXTCOORDs == nullptr) {
		return false;
	}
	else {
		for (unsigned int vertex_index = 0; vertex_index < VERTEX_NUMBER; vertex_index++) {
			if (!
				(POSITIONs[vertex_index].length() > 0.0f && NORMALs[vertex_index].length() > 0.0f && TANGENTs[vertex_index].length() > 0.0f && BITANGENTs[vertex_index].length() > 0.0f && TEXTCOORDs[vertex_index].length() > 0.0f)
				) {
				//There is no initialized vertex attribute for this vertex, at least vertex normals should point somewhere!
				return false;
			}
		}
	}
	if (INDICEs_LENGTH <= 0 || INDICEs == nullptr) {
		return false;
	}
	else {
		for (unsigned int indice_index = 0; indice_index < INDICEs_LENGTH; indice_index++) {
			if (INDICEs[indice_index] < 0) {
				return false;
			}
		}
	}
	return true;
}


//Static Model Data class function definitions

Static_Model_Data::Static_Model_Data(Static_Mesh_Data** mesh_array, unsigned int mesh_number, unsigned int id, string name) {
	//First, store the model's pointer!
	ALL_MODEL_DATAs.push_back(this);

	MESH_ARRAY_PTR = mesh_array;
	MESH_NUMBER = mesh_number;
	ID = id;
	NAME = name;
}

Static_Model_Data::Static_Model_Data() : Resource_Type() {
	//First, store the model's pointer!
	ALL_MODEL_DATAs.push_back(this);
}

//Returns true if verified successfully
//Checks mesh number correctness; model name's, path's, mesh array pointer's, each mesh in the array's, each vertex attribute of each mesh's existence (Is it null or wrong vs some possible value?)
bool Static_Model_Data::Verify_Resource_Data() {
	if (MESH_NUMBER > 0) {
		if (MESH_ARRAY_PTR != nullptr && NAME != "" && PATH != "") {
			for (unsigned int mesh_index = 0; mesh_index < MESH_NUMBER; mesh_index++) {
				Static_Mesh_Data* MESH = MESH_ARRAY_PTR[mesh_index];
				if (MESH != nullptr) {
					if (!MESH->Verify_Mesh_Data()) {
						//There should be log!
						return false;
					}
				}
			}

			//Here, all of the meshes are verified so we should return true!
			return true;
		}
	}

	//Model isn't verified, any of the Meshes are verified.
	//There should be log!
	return false;
}


		//GETTER-SETTERs

unsigned int Static_Model_Data::Get_Mesh_Number() {
	return MESH_NUMBER;
}
Static_Mesh_Data* Static_Model_Data::Get_Mesh_byIndex(unsigned int index) {
	return MESH_ARRAY_PTR[index];
}
void Static_Model_Data::Set_ID_and_NAME(unsigned int id, string name) {
	ID = id;
	NAME = name;
}

Static_Model_Data* Static_Model_Data::Find_Model_byID(unsigned int ID) {
	for (Static_Model_Data* MODEL : Static_Model_Data::ALL_MODEL_DATAs) {
		if (ID == MODEL->ID) {
			return MODEL;
		}
	}
	cout << "ERROR: Intended Model isn't found by ID!: " << ID << endl;
	TuranAPI::Breakpoint();
}

TuranAPI::TuranAPI_ENUMs Static_Model_Data::Get_Resource_Type() {
	return TuranAPI_ENUMs::STATIC_MODEL_RESOURCE;
}


const vec3* Static_Mesh_Data::Get_Positions() const {
	return POSITIONs;
}

const unsigned int* Static_Mesh_Data::Get_Indices() const {
	return INDICEs;
}

const vec3* Static_Mesh_Data::Get_Normals() const {
	return NORMALs;
}

const vec2* Static_Mesh_Data::Get_TextCoords() const {
	return TEXTCOORDs;
}

const vec3* Static_Mesh_Data::Get_Tangents() const {
	return TANGENTs;
}

const vec3* Static_Mesh_Data::Get_Bitangents() const {
	return BITANGENTs;
}

int Static_Mesh_Data::Get_Vertex_Number() const {
	return VERTEX_NUMBER;
}

int Static_Mesh_Data::Get_Indice_Number() const {
	return INDICEs_LENGTH;
}

void Static_Mesh_Data::Set_GFXI_Mesh(void* gpu_mesh) {
	GFXI_MESH = gpu_mesh;
}