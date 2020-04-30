#include "StaticModel_Component.h"

namespace TuranEditor {


	StaticModel_Component::StaticModel_Component(Static_Model_Data* model) : MATERIALs(LASTUSEDALLOCATOR, 1, 1) {
		MODEL = model;

		//Add a material instance for each mesh of the model!
		for (unsigned int i = 0; i < MODEL->MESHes.size(); i++) {
			MATERIALs.push_back(nullptr);
		}
	}
}