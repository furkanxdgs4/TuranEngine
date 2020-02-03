#include "Game_RenderGraph.h"
#include "Draw Passes/Draw_Passes.h"

Game_RenderGraph::Game_RenderGraph() : RenderGraph() {
	TuranAPI::LOG_STATUS("Game_RenderGraph object is created!");
}

void Game_RenderGraph::Run_RenderGraph() {
	if (!Is_Resources_Setup) {
		TuranAPI::LOG_CRASHING("RenderGraph resources aren't setup!");
		return;
	}
	TuranAPI::LOG_STATUS("Running Game_RenderGraph!");
	//DRAW_PASSes[0]->Render_Loop();
	
	//Display the final result on main window!
	//GFX_API::RENDERER->Show_RenderTarget_onWindow(GFX_API::ONSCREEN_Windows[0], DRAW_PASSes[0]->Get_Framebuffer()->BOUND_RTs[0]);
	TuranAPI::LOG_STATUS("Finished running the Game_RenderGraph!");
}