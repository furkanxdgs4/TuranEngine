#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"

class Game_RenderGraph : public GFX::RenderGraph
{
public:
	Game_RenderGraph();
	virtual void Run_RenderGraph() override;
};

