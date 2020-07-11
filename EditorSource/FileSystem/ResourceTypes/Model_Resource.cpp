#include "Model_Resource.h"
#include <flatbuffers/flatbuffers.h>
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include "Resource_Identifier.h"

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



	//Static Model Data class function definitions

	Static_Model::Static_Model() {

	}

	vector<unsigned int> Static_Model::Upload_toGPU() {
		vector<unsigned int> MeshBuffer_IDs;
		for (unsigned int i = 0; i < MESHes.size(); i++) {
			const Static_Mesh_Data* MESH = MESHes[i];
			unsigned int MESHBUFFER_ID = GFXContentManager->Upload_MeshBuffer(MESH->DataLayout, MESH->DATA, MESH->DATA_SIZE, MESH->VERTEX_NUMBER);
			MeshBuffer_IDs.push_back(MESHBUFFER_ID);
		}
		return MeshBuffer_IDs;
	}
}