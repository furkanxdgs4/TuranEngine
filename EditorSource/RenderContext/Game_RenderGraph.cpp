#include "Game_RenderGraph.h"
#include "Draw Passes/Draw_Passes.h"

#include "GFXSource/GFX_Core.h"

namespace TuranEditor {


	Game_RenderGraph::Game_RenderGraph() : RenderGraph("Game RenderGraph") {
		TuranAPI::LOG_STATUS("Game RenderGraph object is created!");
		vector<GFX_API::Framebuffer::RT_SLOT> MainDrawPass_SLOTs;
		GFX_API::DrawPass* MainPass = new Main_DrawPass(DrawCalls, RenderComponents, MainDrawPass_SLOTs);
		RENDER_NODEs.push_back(MainPass);
		

		MainDrawPass_SLOTs[0].RT_ID = GFXContentManager->Create_RenderTarget(1920, 1080, GFX_API::TEXTURE_DIMENSIONs::TEXTURE_2D
			, GFX_API::TEXTURE_TYPEs::COLORTEXTURE, GFX_API::UNIFORMTYPE::VAR_UBYTE8, true);
		MainDrawPass_SLOTs[1].RT_ID = GFXContentManager->Create_RenderTarget(1920, 1080, GFX_API::TEXTURE_DIMENSIONs::TEXTURE_2D
			, GFX_API::TEXTURE_TYPEs::DEPTHTEXTURE, GFX_API::UNIFORMTYPE::VAR_FLOAT32, true);
		MainPass->RenderGraph_SetupPhase(MainDrawPass_SLOTs);
	}

	void Game_RenderGraph::Run_RenderGraph() {
		if (RENDER_NODEs.size() == 0) {
			TuranAPI::LOG_CRASHING("RenderGraph isn't constructed properly, there is no Render Node!");
			return;
		}


		TuranAPI::LOG_STATUS("Running Game_RenderGraph!");
		GFX_API::DrawPass* MainPass = (GFX_API::DrawPass*)RENDER_NODEs[0];
		MainPass->ResourceUpdatePhase();
		MainPass->Execute();

		//Display the final result on main window!
		//GFX_API::RENDERER->Show_RenderTarget_onWindow(GFX_API::ONSCREEN_Windows[0], DRAW_PASSes[0]->Get_Framebuffer()->BOUND_RTs[0]);
		TuranAPI::LOG_STATUS("Finished running the Game_RenderGraph!");
	}
}