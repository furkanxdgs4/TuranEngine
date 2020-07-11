#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"

/*
	All resources have GL_ID variable. This variable is a pointer to GL specific structure that's used by GL functions (For example; unsigned ints for Textures, Buffers etc. in OpenGL)
	
*/



namespace GFX_API {
	//Create Render Targets as textures
	struct GFXAPI RenderTarget {
	public:
		TEXTURE_DIMENSIONs DIMENSION;
		TEXTURE_TYPEs FORMAT;
		UNIFORMTYPE FORMAT_VALUETYPE;
		unsigned int ID, WIDTH, HEIGHT;
		bool Usable_as_Texture;
		void* GL_ID;
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
		void* GL_ID;
		vector<RT_SLOT> BOUND_RTs;

		Framebuffer();
	};

	struct GFXAPI GFX_Mesh {
		unsigned int BUFFER_ID;
		void* GL_ID;	//In OpenGL4 we have to use VAO, VBO and EBO so this points to OGL4_MESH structure that holds all of these. 
						//I still don't know about buffers in Vulkan, so that's all for now
	};

	struct GFXAPI GFX_Texture {
		unsigned int ASSET_ID;
		void* GL_ID;
	};

	struct GFXAPI GFX_ShaderSource {
		unsigned int ASSET_ID;
		void* GL_ID;
	};

	struct GFXAPI GFX_ShaderProgram {
		unsigned int ASSET_ID;
		void* GL_ID;
	};

	struct GFXAPI RenderState {
		DEPTH_MODEs DEPTH_MODE;
		DEPTH_TESTs DEPTH_TEST_FUNC;
		//There should be Stencil and Blending options, but for now leave it like this
	};
}