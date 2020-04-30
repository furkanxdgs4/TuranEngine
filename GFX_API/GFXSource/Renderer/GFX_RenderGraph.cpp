#include "GFX_RenderGraph.h"

namespace GFX_API {
	//Getter-Setters
	DrawPass::DrawPass(const Vector<DrawCall>& RG_DrawCallBuffer_, const Vector<GFX_API::RenderingComponent>& RG_RenderComponentsBuffer_, const char* name): NAME(name),
		DrawCallBuffer(LASTUSEDALLOCATOR, 100, 100), RenderComponents(LASTUSEDALLOCATOR, 10, 100), Is_SetupPhase_Called(false)
		, RG_DrawCallBuffer(RG_DrawCallBuffer_), RG_RenderComponentBuffer(RG_RenderComponentsBuffer_){

	}

	const char* DrawPass::Get_Name() {
		return NAME;
	}


	RenderGraph::RenderGraph(const char* name) : RENDER_NODEs(LASTUSEDALLOCATOR, 1, 1), DrawCalls(LASTUSEDALLOCATOR, 100, 1000), RenderComponents(LASTUSEDALLOCATOR, 100, 1000), NAME(name){
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
}