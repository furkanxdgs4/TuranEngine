#pragma once
#include "GFXSource/GFX_Includes.h"
#include "TuranAPI/API_FileSystem.h"

#include "GFX_Resource.h"
#include "GFX_RenderCommands.h"
#include "GFX_RenderGraph.h"

namespace GFX {
	//Manager should check RenderGraph's Render Nodes and Scene's Components
	//Also Manager should manage RenderGraph's Render Node order, Transient Resources etc.
	//But Resource Manager should be Graphics API specific, because OpenGL is not good for Transient Resources for example.
	class RenderGraph_Manager {
		RenderGraph* RENDERGRAPH;
	public:
		//Bind a RenderGraph to 
		void Bind_RenderGraph(RenderGraph* RenderGraph);

		//This will be responsible for creating as less resources as possible for RenderGraph!
		virtual void Create_RenderGraphResources() = 0;
		//This will be responsible for creating gpu resources for RenderGraphs and giving them to RenderGraph
		virtual void Manage_SceneResources() = 0;
		//This will be responsible for reordering Render Nodes for transient resources etc.
		virtual void Manage_RenderGraph() = 0;
		virtual void Destroy_RenderGraph() = 0;
		//Prepare RenderGraph for new frame. For example Mipmap History Buffers, erase unnecessary things from memory etc
		virtual void NewFrame_Setup() = 0;
	};



	class GFXAPI Renderer {
	protected:
		static vector<RenderGraph*> Bound_RenderGraphs;
		static vector<unsigned int> RenderGraphIDs_toRunThisFrame;
		static RenderGraph_Manager* GraphManager;

		static unsigned int Find_RenderGraph_VectorIndex(unsigned int RenderGraph_ID);
		static RenderGraph* Find_RenderGraph_byID(unsigned int RenderGraph_ID);
		static unsigned int RenderGraph_ID_Generation();
	public:
		friend class GFX_Core;

		~Renderer();
		void* Renderer_Context;
		static Renderer* SELF;


		//RENDERER FUNCTIONs

		static void Bind_RenderGraph(RenderGraph* RenderGraph);
		static void Destroy_RenderGraph(unsigned int RenderGraph_ID);
		static void Run_RenderGraph_ThisFrame(unsigned int RenderGraph_ID);
		static void New_Frame();
	};
}