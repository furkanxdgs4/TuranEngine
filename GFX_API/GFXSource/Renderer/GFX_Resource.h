#pragma once
#include "GFXSource/GFX_Includes.h"


namespace GFX {
	//Create Render Targets as textures
	struct GFXAPI RenderTarget {
	private:
		unsigned int ID;
		TuranAPI::TuranAPI_ENUMs DIMENSION;
		GFX_ENUM FORMAT;
		GFX_ENUM ATTACHMENT;
		TuranAPI::TuranAPI_ENUMs FORMAT_VALUETYPE;
	public:
		unsigned int Get_ID() const;
	};



	//This is a storage struct that has no functions
	//Changing this doesn't affect the actual framebuffer until one of member variables is used!
	//Because of that, a framebuffer should only created by a Draw Pass
	//Please don't change any object by yourself, GFX has enough functions to change framebuffers
	//If you are creating your own render pipeline or whatsoever, you can use this storage class because GFX doesn't store all objects of this class
	//Only Draw Passes has framebuffers and Windows has their own by default because of OpenGL3 drivers!
	struct GFXAPI Framebuffer {
	public:

		unsigned int ID;
		vector<RenderTarget*> BOUND_RTs;

		unsigned int MSAA_spp;
		unsigned int WIDTH, HEIGHT;
		Framebuffer(unsigned int width, unsigned int height);
	};



	struct GFXAPI GPU_MESH {
	public:
		static vector<GPU_MESH*> ALL_GPU_MESHes;

		unsigned int VAO, VBO, EBO;
		unsigned int Indices_Number, Vertex_Number;
		GPU_MESH();
	};




	struct GFXAPI RenderState {
		GFX_ENUM DEPTH_MODE;
		GFX_ENUM DEPTH_TEST_FUNC;
		//There should be Stencil and Blending options, but for now leave it like this
	};


}