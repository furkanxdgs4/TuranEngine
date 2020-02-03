#include "StaticModel_Component.h"

using namespace TuranAPI::Game_Object;

StaticModel_Component::StaticModel_Component(TuranAPI::File_System::Static_Model_Data* model) {
	MODEL = model;

	//Add a material instance for each mesh of the model!
	for (unsigned int i = 0; i < MODEL->Get_Mesh_Number(); i++) {
		MATERIALs.push_back(nullptr);
	}
}

TuranAPI::TuranAPI_ENUMs StaticModel_Component::Get_Component_Type() {
	return TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_COMP;
}