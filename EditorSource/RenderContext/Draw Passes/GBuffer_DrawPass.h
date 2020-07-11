#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/Renderer/GFX_Resource.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"


class Main_DrawPass : public GFX_API::DrawPass {
public:
	Main_DrawPass(const vector<GFX_API::DrawCall>& RG_DrawCalls, const vector<GFX_API::RenderingComponent>& RG_RenderComponents, vector<GFX_API::Framebuffer::RT_SLOT>& Needed_RTSlots);

	static unsigned int Get_BitMaskFlag();

	virtual void RenderGraph_SetupPhase(vector<GFX_API::Framebuffer::RT_SLOT>& RTs) override;
	virtual void ResourceUpdatePhase() override;
	virtual void Execute() override;
};
