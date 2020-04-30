#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"


namespace GFX_API {
	//Create Render Targets as textures
	struct GFXAPI RenderTarget {
	public:
		TEXTURE_DIMENSIONs DIMENSION;
		TEXTURE_TYPEs FORMAT;
		UNIFORMTYPE FORMAT_VALUETYPE;
		unsigned int ID, WIDTH, HEIGHT;
		bool Usable_as_Texture;
	};




	struct GFXAPI Framebuffer {
		struct RT_SLOT {
			unsigned int RT_ID, WIDTH, HEIGTH;
			RT_ATTACHMENTs ATTACHMENT_TYPE;
			OPERATION_TYPE RT_OPERATIONTYPE;
			RT_READSTATE RT_READTYPE;
			vec3 CLEAR_COLOR;
		};
	public:
		unsigned int ID;
		Vector<RT_SLOT> BOUND_RTs;

		Framebuffer();
	};


	struct GFXAPI RenderState {
		DEPTH_MODEs DEPTH_MODE;
		DEPTH_TESTs DEPTH_TEST_FUNC;
		//There should be Stencil and Blending options, but for now leave it like this
	};
}