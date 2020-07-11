#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_ENUMs.h"
#include "GFX_Resource.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Material_Type_Resource.h"

namespace GFX_API {

	/*
	1) This class will be used to store necessary informations to draw something
	2) For example, Draw_StaticObject(Material_Instance*,Static_ModelData*) struct will store Material's RenderState, Instance's Uniform Pointers, Static_ModelData's GPU and CPU mesh pointers.
		*Then RenderGraph_Manager will read these informations and creates data dependencies. Then edits the RenderGraph and turns RenderCommands to efficient APICommands
	*/
	struct GFXAPI DrawCall {
		unsigned int JoinedDrawPasses;	//This is a mask for each Draw Pass! 
		//Each Draw Pass will check this first, then ShaderPipeline to check if this Draw Call suits its support range.
		//Unsigned int is 32 bit, so the maximum number of draw passes in a RenderGraph is 32 bit
		unsigned int MeshBuffer_ID;
		unsigned int ShaderInstance_ID;
	};

	struct GFXAPI RenderingComponent {
		unsigned short Type_and_Key; //This will indicate a Camera or Particle type, then other bits will be used to detect which component we are trying to access.
		//First 5 bit is for the type, other 11 bit is for the key.
		unsigned int Buffer_ID; //This data should be uploaded to the GPU and ready to set as input to API calls.
		//For example; if this is a Camera, GPUData is the pointer to GPUCameraData that has been uploaded to GPU by Editor!
	};



	struct GFXAPI RendererState {
		unsigned int Active_ShaderInstanceID, Active_FramebufferID;
		DEPTH_TESTs DEPTH_TEST; DEPTH_MODEs DEPTH_MODE;
		BLEND_STATEs BLEND_STATE;
	};
}