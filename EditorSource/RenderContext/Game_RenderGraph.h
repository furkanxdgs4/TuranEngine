#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"

namespace TuranEditor {
	class Game_RenderGraph : public GFX_API::RenderGraph
	{
	public:
		Game_RenderGraph();
		virtual void Run_RenderGraph() override;
	};
}