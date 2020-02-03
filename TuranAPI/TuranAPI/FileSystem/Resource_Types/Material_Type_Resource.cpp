#include "Material_Type_Resource.h"

using namespace TuranAPI::File_System;

Material_Uniform::Material_Uniform(string variable_name, TuranAPI::TuranAPI_ENUMs variable_type) : VARIABLE_NAME(variable_name), VARIABLE_TYPE(variable_type) {}
Material_Uniform::Material_Uniform() {}

//Please keep in mind that: This doesn't verifies if data matches with variable type!
bool Material_Uniform::Verify_UniformData() {
	if (DATA != nullptr && VARIABLE_TYPE != TuranAPI_ENUMs::TURAN_NULL && VARIABLE_NAME != "") {
		return true;
	}
	return false;
}

vector<Material_Instance*> Material_Instance::ALL_Material_Instances = vector<Material_Instance*>{};

vector<Material_Type_Resource*> Material_Type_Resource::ALL_MATERIAL_TYPEs = vector<Material_Type_Resource*>{};

TuranAPI::TuranAPI_ENUMs Material_Instance::Get_Resource_Type() {
	return TuranAPI::TuranAPI_ENUMs::MATERIAL_INSTANCE_RESOURCE;
}

Material_Type_Resource::Material_Type_Resource() {
	ALL_MATERIAL_TYPEs.push_back(this);
}

TuranAPI::TuranAPI_ENUMs Material_Type_Resource::Get_Resource_Type() {
	return TuranAPI_ENUMs::MATERIAL_TYPE_RESOURCE;
}

bool Material_Type_Resource::Verify_Resource_Data() {
	if (GFX_API != TuranAPI_ENUMs::TURAN_NULL && VERTEX_SOURCE != "" && FRAGMENT_SOURCE != "" && NAME != "" && PATH != "") {
		for (unsigned int uniform_index = 0; uniform_index < UNIFORMs.size(); uniform_index++) {
			Material_Uniform* UNIFORM = &UNIFORMs[uniform_index];
			if (!
				(UNIFORM->VARIABLE_TYPE != TuranAPI_ENUMs::TURAN_NULL && UNIFORM->VARIABLE_NAME != "")) {
				//If Material Type's uniform isn't verified!
				//There should be log!
				//A Material Type's uniform doesn't hold any data, so UNIFORM->Verify_UniformData is meaningless!

				cout << "Material Type isn't verified because of a uniform has a uninitialized Variable Type or Variable Name!";
				TuranAPI::Breakpoint();
				return false;
			}
		}
		//Material type's every uniform is verified!
		return true;
	}
	else {
		cout << "Material Type isn't verified because of GFX_API or Vertex Source or Fragment Source or Resource Name or Resource Path!";
		TuranAPI::Breakpoint();
		return false;
	}
	return false;
}









Material_Instance::Material_Instance() {
	ALL_Material_Instances.push_back(this);
}

void Material_Instance::Set_Uniform_Data(string uniform_name, void* pointer_to_data) {
	Material_Uniform* uniform = &UNIFORM_LIST[Find_Uniform_byName(uniform_name)];
	if (pointer_to_data == nullptr) {
		cout << "Error: Couldn't set GPU uniform data, because data is nullptr!\n";
		TuranAPI::Breakpoint();
		return;
	}
	if (uniform == nullptr) {
		cout << "Error: Found uniform is nullptr!\n";
		TuranAPI::Breakpoint();
		return;
	}
	uniform->DATA = pointer_to_data;
}

bool Material_Instance::Verify_Resource_Data() {
	if (Material_Type != nullptr && NAME != "" && PATH != "") {
		if (Material_Type->Verify_Resource_Data()) {
			if (UNIFORM_LIST.size() == Material_Type->UNIFORMs.size()) {
				for (unsigned int uniform_index = 0; uniform_index < UNIFORM_LIST.size(); uniform_index++) {
					Material_Uniform* UNIFORM = &UNIFORM_LIST[uniform_index];

					//Note: There should be uniform data storing for Material Instances
				}
			}
			else {
				cout << "Material Instance isn't verified because Uniform lists isn't match between Material Type and Instance!";
				TuranAPI::Breakpoint();
				return false;
			}
		}
		else {
			cout << "Material Instance isn't verified because Material Type isn't verified!";
			TuranAPI::Breakpoint();
			return false;
		}
	}
	else {
		cout << "Material Instance isn't verified because of Material Type or Resource Name or Resource Path!";
		TuranAPI::Breakpoint();
		return false;
	}
	return true;
}

unsigned int Material_Instance::Find_Uniform_byName(string uniform_name) {
	for (unsigned int i = 0; i < UNIFORM_LIST.size(); i++) {
		if (UNIFORM_LIST[i].VARIABLE_NAME == uniform_name)
			return i;
	}
	cout << "Error: Intended uniform variable: " << uniform_name << " can't be found in Material Type: " << Material_Type->NAME << endl;
	TuranAPI::Breakpoint();
	return -1;
}

Material_Instance* Material_Instance::Find_MaterialInstance_byID(unsigned int ID) {
	for (Material_Instance* MAT_INST : ALL_Material_Instances) {
		if (MAT_INST->ID == ID) {
			return MAT_INST;
		}
	}
	cout << "Error: Intended Material Instance isn't found by ID!: " << ID << endl;
	TuranAPI::Breakpoint();
	return nullptr;
}

Material_Instance* Material_Instance::Find_MaterialInstance_byName(string name_of_resource) {
	for (Material_Instance* MAT_INST : ALL_Material_Instances) {
		if (MAT_INST->NAME == name_of_resource) {
			return MAT_INST;
		}
	}
	cout << "Error: Intended Material Instance: " << name_of_resource << " isn't found!\n";
	TuranAPI::Breakpoint();
	return nullptr;
}