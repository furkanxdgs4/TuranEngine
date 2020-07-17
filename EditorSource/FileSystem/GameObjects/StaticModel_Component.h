#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {

	/* There are some abstractions and lacks in this class, here is some definitions:
	1) In a model, each part that has different material index is created as a mesh according to .obj file!
	2) So, each model consists of meshes!
	3) This means you would specify each mesh's shader in this class, but I don't want to complicate this anymore!
	4) So, for now, each mesh of the model uses the same material/shader and each model has one material/shader!
	5) There is no difference between the terms "material" and "shader", because there isn't a material system in engine!
	*/
	class StaticModel_Component : public TuranAPI::GameComponent {
	public:
		mat4 Model_Transform;
		Resource_Identifier* MODEL;
		vector<Resource_Identifier*> MATERIALs;
		StaticModel_Component();
	};
}
