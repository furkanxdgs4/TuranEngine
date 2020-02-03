#pragma once
#include "GFXSource/GFX_Includes.h"
#include "TuranAPI/FileSystem/Resource_Types/Scene_Resource.h"

//Mesh Library
//#include "Graphics/Meshes/Mesh_HEADER.h"

#include "GFX_Resource.h"

namespace GFX {
	class GFXAPI RenderNode {
		GFX_ENUM RenderNode_Type;
	};


	//If you want to create a Draw Pass:
	//1) You should create a class inherited from this
	//2) Include the class header in GFX API specific header (For example: OGL3_Core)
	//3) Create an object as "new" in GFX API specfic class's Start_Creation()
	//4) Don't forget to give a name to the draw pass! It will show up in Framegraph, Debugging and Profiling!
	class GFXAPI DrawPass : public RenderNode {
		friend class Renderer;
		friend class OGL3_Renderer;

	protected:
		DrawPass();

		//Framebuffer stores render targets too, see the code
		Framebuffer* FRAMEBUFFER;
		string NAME;
		GFX_ENUM DEPTHTEST_MODE;
		GFX_ENUM DEPTHBUFFER_MODE;
	public:
		//FUNCTIONALITIES FOR EACH DRAW PASS!

		virtual void Creation() = 0;
		virtual void Update_Resources() = 0;
		virtual void Render_Loop() = 0;

		//GETTER-SETTERs

		Framebuffer* Get_Framebuffer();
		string Get_Name();
	};



	class GFXAPI RenderGraph {
	protected:
		bool Is_Resources_Setup = false, Is_RenderNodes_Managed = false;
		vector<RenderNode*> RENDER_NODEs;
		RenderTarget* FINAL_COLOR_TEXTURE;
		unsigned int RenderGraph_ID;
	public:
		static TuranAPI::File_System::Scene_Resource* Scene;
		string NAME;

		virtual void Run_RenderGraph() = 0;

		//GETTER-SETTERs

		void Set_RenderGraphID(unsigned int ID);
		unsigned int Get_RenderGraph_ID() const;
		const RenderTarget* Get_FinalColor_Texture() const;

		RenderGraph();
	};
}