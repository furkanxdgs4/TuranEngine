#pragma once

//This header is used to create a GL data type classification for all of the TuranEngine
//For example; Texture types, Mipmap settings, Desktop modes etc.
//In GL specific [GFX_API]_ENUMs.h, these data types have to be converted to GL specific data types
//For example; GFX_TEXTURE_2D data type here, should be converted to GL_TEXTURE_2D in OGL3_ENUMs.h
namespace GFX {
	enum GFX_ENUM : unsigned short {
		GFX_NULL = 0,

		//Window modes

		FULLSCREEN = 1,
		WINDOWED,

		//V-Sync Modes

		V_SYNC_OFF,
		V_SYNC_ON,

		//Texture Formats for Render Targets

		//This format should use RGBA channels in Turan Engine
		COLORTEXTURE_FORMAT,
		DEPTHTEXTURE_FORMAT,

		//Render Target Attachments

		TEXTURE_ATTACHMENT_COLOR0,
		TEXTURE_ATTACHMENT_DEPTH,

		//Depth Test Functions

		DEPTH_TEST_ALWAYS,
		DEPTH_TEST_NEVER,
		DEPTH_TEST_LESS,
		DEPTH_TEST_LEQUAL,
		DEPTH_TEST_GREATER,
		DEPTH_TEST_GEQUAL,

		//Depth Mode

		DEPTH_READ_WRITE,
		DEPTH_READ_ONLY,
		DEPTH_OFF,

		//Render Node Types
		RENDERNODE_DRAWPASS,
		RENDERNODE_COMPUTE,
		RENDERNODE_COPYPASS
	};

}