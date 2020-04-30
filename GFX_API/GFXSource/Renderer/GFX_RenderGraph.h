#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFX_APICommands.h"
#include "GFX_RenderCommands.h"
#include "GFX_Resource.h"

namespace GFX_API {
	class GFXAPI RenderNode {
		RENDERNODE_TYPEs RenderNode_Type;
	};


	//If you want to create a Draw Pass:
	//1) You should create a class inherited from this
	//2) Include the class header in GFX API specific header (For example: OGL3_Core)
	//3) Create an object as "new" in GFX API specfic class's Start_Creation()
	//4) Don't forget to give a name to the draw pass! It will show up in Framegraph, Debugging and Profiling!
	class GFXAPI DrawPass : public RenderNode {
	protected:
		friend class Renderer;
		friend class OGL4_Renderer;

		//These Vectors store indexes to the RenderGraph's Vectors!
		Vector<unsigned int> DrawCallBuffer;
		Vector<unsigned int> RenderComponents;
		const Vector<DrawCall>& RG_DrawCallBuffer;
		const Vector<RenderingComponent>& RG_RenderComponentBuffer;

		unsigned int FRAMEBUFFER;
		bool Is_SetupPhase_Called;
		String NAME;
	public:
		DrawPass(const Vector<DrawCall>& RG_DrawCallBuffer, const Vector<GFX_API::RenderingComponent>& RG_RenderComponents, const char* NAME);

		//While constructing a RenderGraph, call this to couple DrawPass and RenderGraph
		//So, DrawCallBuffer should point RenderGraph's DrawCalls Vector!
		virtual void RenderGraph_SetupPhase(Vector<GFX_API::Framebuffer::RT_SLOT>& RTs) = 0;
		//Update DrawCallBuffer!
		virtual void ResourceUpdatePhase() = 0;
		virtual void Execute() = 0;

		//GETTER-SETTERs

		const char* Get_Name();
	};


	class GFXAPI RenderGraph {
	protected:
		Vector<RenderNode*> RENDER_NODEs;
		Vector<DrawCall> DrawCalls;
		Vector<RenderingComponent> RenderComponents;
		unsigned int RenderGraph_ID;
	public:
		String NAME;
		RenderGraph(const char* name);

		virtual void Run_RenderGraph() = 0;
		void Register_DrawCall(DrawCall drawcall);

		//GETTER-SETTERs

		void Set_RenderGraphID(unsigned int ID);
		unsigned int Get_RenderGraph_ID() const;
	};
}