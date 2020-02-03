#include "GFX_Renderer.h"

namespace GFX {
	void RenderGraph_Manager::Bind_RenderGraph(RenderGraph* RenderGraph) { RENDERGRAPH = RenderGraph; TuranAPI::LOG_STATUS("RenderGraph: " + RenderGraph->NAME + " is bound to RenderGraph Manager!"); }


	Renderer* Renderer::SELF = nullptr;
	vector<RenderGraph*> Renderer::Bound_RenderGraphs = vector<RenderGraph*>{};
	vector<unsigned int> Renderer::RenderGraphIDs_toRunThisFrame = vector<unsigned int>{};
	RenderGraph_Manager* Renderer::GraphManager = nullptr;

	Renderer::~Renderer() {
		TuranAPI::LOG_NOTCODED("GFX_Renderer Destruction isn't coded!", false);
	}

	void Renderer::Bind_RenderGraph(RenderGraph* RenderGraph) {
		Bound_RenderGraphs.push_back(RenderGraph);
		RenderGraph->Set_RenderGraphID(RenderGraph_ID_Generation());
	}

	void Renderer::Destroy_RenderGraph(unsigned int RenderGraph_ID) {
		RenderGraph* RenderGraph = Find_RenderGraph_byID(RenderGraph_ID);
		GraphManager->Bind_RenderGraph(RenderGraph);
		GraphManager->Destroy_RenderGraph();
		unsigned int vector_index = Find_RenderGraph_VectorIndex(RenderGraph->Get_RenderGraph_ID());
		Bound_RenderGraphs.erase(Bound_RenderGraphs.begin() + vector_index);
	}
	void Renderer::Run_RenderGraph_ThisFrame(unsigned int RenderGraph_ID) {
		//In final production, there will be no situation cause this error! So, run this in debug build
#ifdef TURAN_DEBUGGING
		for (unsigned int ID : RenderGraphIDs_toRunThisFrame) {
			if (RenderGraph_ID == ID) {
				TuranAPI::LOG_ERROR("Renderer::Run_RenderGraph_ThisFrame is called more than one for RenderGraph_ID: " + to_string(RenderGraph_ID));
				return;
			}
		}
#endif
		RenderGraphIDs_toRunThisFrame.push_back(RenderGraph_ID);
	}
	void Renderer::New_Frame() {
		TuranAPI::LOG_STATUS("Renderer prepares for this frame!");

		RenderGraphIDs_toRunThisFrame.clear();
		for (RenderGraph* RENDERGRAPH : Bound_RenderGraphs) {
			GraphManager->Bind_RenderGraph(RENDERGRAPH);
			GraphManager->NewFrame_Setup();
		}
	}



	
	//Helper Functions

	unsigned int Renderer::Find_RenderGraph_VectorIndex(unsigned int RenderGraph_ID) {
		for (unsigned int index = 0; index < Bound_RenderGraphs.size(); index++) {
			RenderGraph* RENDERGRAPH = Bound_RenderGraphs[index];
			if (RenderGraph_ID == RENDERGRAPH->Get_RenderGraph_ID()) {
				return index;
			}
		}
		TuranAPI::LOG_ERROR("Renderer::Find_RenderGraph_VectorIndex() failed to find RenderGraph_ID: " + to_string(RenderGraph_ID));
		return 0;
	}
	RenderGraph* Renderer::Find_RenderGraph_byID(unsigned int RenderGraph_ID) {
		for (RenderGraph* RENDERGRAPH : Bound_RenderGraphs) {
			if (RenderGraph_ID == RENDERGRAPH->Get_RenderGraph_ID()) {
				return RENDERGRAPH;
			}
		}
		TuranAPI::LOG_ERROR("Renderer::Find_RenderGraph_byID failed to find a RenderGraph by ID: " + to_string(RenderGraph_ID));
		return nullptr;
	}
	unsigned int Renderer::RenderGraph_ID_Generation() {
		TuranAPI::LOG_NOTCODED("Renderer::RenderGraph_ID_Generation() isn't coded!", true);
		return 0;
	}
}