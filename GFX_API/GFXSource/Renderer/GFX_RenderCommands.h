#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFX_Resource.h"

namespace GFX {
	namespace RenderCommands {
		/*
		1) This class will be used to store necessary informations to draw something
		2) For example, Draw_StaticObject(Material_Instance*,Static_ModelData*) struct will store Material's RenderState, Instance's Uniform Pointers, Static_ModelData's GPU and CPU mesh pointers.
			*Then RenderGraph_Manager will read these informations and creates data dependencies. Then edits the RenderGraph and turns RenderCommands to efficient APICommands
		*/
		struct GFXAPI RenderCommand {
			unsigned int Command_ID;
		};

		struct GFXAPI Draw_StaticModel : public RenderCommand {
			RenderState* State;
			//UniformBuffers* MaterialInstance_UniformList
			//Static_ModelData* ModelData;
		};
	}
}