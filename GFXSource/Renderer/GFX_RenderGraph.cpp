#include "GFX_RenderGraph.h"

namespace GFX_API {
	//Getter-Setters
	DrawPass::DrawPass(const vector<DrawCall>& RG_DrawCallBuffer_, const vector<GFX_API::RenderingComponent>& RG_RenderComponentsBuffer_, const char* name): NAME(name),
		Is_SetupPhase_Called(false)
		, RG_DrawCallBuffer(RG_DrawCallBuffer_), RG_RenderComponentBuffer(RG_RenderComponentsBuffer_){

	}

	const char* DrawPass::Get_Name() {
		return NAME.c_str();
	}

	unsigned int DrawPass::Get_FramebufferID() {
		return FRAMEBUFFER;
	}


	RenderGraph::RenderGraph(const char* name) : NAME(name){
		TuranAPI::LOG_STATUS("Render Graph object is created!");
	}

	void RenderGraph::Set_RenderGraphID(unsigned int ID) {
		RenderGraph_ID = ID;
		//TuranAPI::LOG_STATUS("RenderGraph: " + NAME + "'s ID is set as " + to_string(ID));
	}
	unsigned int RenderGraph::Get_RenderGraph_ID() const {
		return RenderGraph_ID;
	}
	void RenderGraph::Register_DrawCall(DrawCall drawcall) {
		DrawCalls.push_back(drawcall);
	}
	const vector<const RenderNode*> RenderGraph::Get_RenderNodes() {
		vector<const RenderNode*> RNs;
		for (RenderNode* RN : RENDER_NODEs) {
			RNs.push_back(RN);
		}
		return RNs;
	}
}