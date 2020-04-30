#pragma once
#include "GFX_Includes.h"

//This header is used to create a GL data type classification for all of the TuranEngine
//For example; Texture types, Mipmap settings, Desktop modes etc.
//In GL specific [GFX_API]_ENUMs.h, these data types have to be converted to GL specific data types
//For example; GFX_TEXTURE_2D data type here, should be converted to GL_TEXTURE_2D in OGL3_ENUMs.h
namespace GFX_API {
	//Variable Types!
	enum class UNIFORMTYPE : unsigned char {
		VAR_UBYTE8 = 0, VAR_BYTE8, VAR_UINT32, VAR_INT32, VAR_FLOAT32,
		VAR_VEC2, VAR_VEC3, VAR_VEC4, VAR_MAT4x4, TEXTURE2D
	};
	GFXAPI unsigned int Get_UNIFORMTYPEs_SIZEinbytes(UNIFORMTYPE uniform);
	GFXAPI const char* Find_UNIFORM_VARTYPE_Name(UNIFORMTYPE uniform_var_type);

	enum class RT_ATTACHMENTs : unsigned char {
		TEXTURE_ATTACHMENT_COLOR0,
		TEXTURE_ATTACHMENT_DEPTH,
		TEXTURE_ATTACHMENT_DEPTHSTENCIL
	};

	enum class OPERATION_TYPE : unsigned char {
		READ_ONLY,
		WRITE_ONLY,
		READ_AND_WRITE
	};

	enum class RT_READSTATE : unsigned char {
		CLEAR,
		DONT_CARE,
		LAST_FRAME
	};

	enum class DEPTH_TESTs : unsigned char {
		DEPTH_TEST_ALWAYS,
		DEPTH_TEST_NEVER,
		DEPTH_TEST_LESS,
		DEPTH_TEST_LEQUAL,
		DEPTH_TEST_GREATER,
		DEPTH_TEST_GEQUAL
	};

	enum class DEPTH_MODEs : unsigned char {
		DEPTH_READ_WRITE,
		DEPTH_READ_ONLY,
		DEPTH_OFF
	};

	enum class RENDERNODE_TYPEs {
		RENDERNODE_DRAWPASS,
		RENDERNODE_COMPUTE,
		RENDERNODE_COPYPASS
	};

	enum class TEXTURE_DIMENSIONs : unsigned char {
		TEXTURE_2D = 0,
		TEXTURE_3D = 1
	};

	enum class TEXTURE_MIPMAPFILTER : unsigned char {
		API_TEXTURE_NEAREST_FROM_1MIP,
		API_TEXTURE_LINEAR_FROM_1MIP,
		API_TEXTURE_NEAREST_FROM_2MIP,
		API_TEXTURE_LINEAR_FROM_2MIP
	};

	enum class TEXTURE_WRAPPING : unsigned char {
		API_TEXTURE_REPEAT,
		API_TEXTURE_MIRRORED_REPEAT,
		API_TEXTURE_CLAMP_TO_EDGE
	};

	enum class TEXTURE_CHANNELs: unsigned char {
		API_TEXTURE_RGB,
		API_TEXTURE_RGBA
	};

	enum class TEXTURE_TYPEs: unsigned char {
		COLORTEXTURE,
		DEPTHTEXTURE,
		DEPTHSTENCIL
	};

	enum class GPU_TYPEs : unsigned char{
		DISCRETE_GPU,
		INTEGRATED_GPU
	};

	enum class V_SYNC : unsigned char {
		VSYNC_OFF,
		VSYNC_DOUBLEBUFFER,
		VSYNC_TRIPLEBUFFER
	};

	enum class WINDOW_MODE : unsigned char {
		FULLSCREEN,
		WINDOWED
	};

	enum class GFX_APIs : unsigned char{
		OPENGL4 = 0,
		VULKAN = 1
	};

	enum class SHADER_LANGUAGEs : unsigned char {
		GLSL	= 0,
		HLSL	= 1,
		SPIRV	= 2,
		TSL		= 3
	};

	enum class SHADER_STAGE : unsigned char {
		VERTEXSTAGE		= 0,

		//There should be TESSELATION etc.

		FRAGMENTSTAGE	= 1,
		COMPUTE			= 2
	};



	//CODE THESE ENUMs IN SUMMER!

	enum class BLEND_STATEs : unsigned char{
		//Code here
	};
}