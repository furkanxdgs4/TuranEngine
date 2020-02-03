#include "GFX_RenderGraph.h"

namespace GFX {
	DrawPass::DrawPass() : FRAMEBUFFER() {}

	//Getter-Setters
	Framebuffer* DrawPass::Get_Framebuffer() {
		return FRAMEBUFFER;
	}

	string DrawPass::Get_Name() {
		return NAME;
	}


	RenderGraph::RenderGraph() {
		TuranAPI::LOG_STATUS("Render Graph object is created!");
	}

	void RenderGraph::Set_RenderGraphID(unsigned int ID) {
		RenderGraph_ID = ID;
		TuranAPI::LOG_STATUS("RenderGraph: " + NAME + "'s ID is set as " + to_string(ID));
	}
	unsigned int RenderGraph::Get_RenderGraph_ID() const {
		return RenderGraph_ID;
	}

	const RenderTarget* RenderGraph::Get_FinalColor_Texture() const {
		return FINAL_COLOR_TEXTURE;
	}
}