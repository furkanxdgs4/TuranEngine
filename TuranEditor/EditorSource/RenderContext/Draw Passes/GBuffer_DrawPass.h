#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/Renderer/GFX_Resource.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"


class GFXAPI G_BUFFER_DrawPass : public GFX::DrawPass {
	virtual void Creation() override;
	virtual void Update_Resources() override;
	virtual void Render_Loop() override;
};
