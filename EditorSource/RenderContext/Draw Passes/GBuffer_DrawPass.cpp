#include "GBuffer_DrawPass.h"
#include "GFXSource/GFX_Core.h"

#include <string>

unsigned int Main_DrawPass::Get_BitMaskFlag() {
	return 1;
}

Main_DrawPass::Main_DrawPass(const vector<GFX_API::DrawCall>& RG_DrawCallBuffer_, const vector<GFX_API::RenderingComponent>& RG_RenderComponentBuffer_, vector<GFX_API::Framebuffer::RT_SLOT>& Needed_RTSlots)
	: DrawPass(RG_DrawCallBuffer_, RG_RenderComponentBuffer_, "Main Pass") {
	GFX_API::Framebuffer::RT_SLOT Color0_SLOT;
	Color0_SLOT.ATTACHMENT_TYPE = GFX_API::RT_ATTACHMENTs::TEXTURE_ATTACHMENT_COLOR0;
	Color0_SLOT.RT_OPERATIONTYPE = GFX_API::OPERATION_TYPE::WRITE_ONLY;
	Color0_SLOT.RT_READTYPE = GFX_API::RT_READSTATE::CLEAR;
	Color0_SLOT.CLEAR_COLOR = vec3(0.5f, 0.4f, 0.6f);
	//These should be set by RenderGraph!
	Color0_SLOT.HEIGTH = 0; Color0_SLOT.WIDTH = 0; Color0_SLOT.RT_ID = 0;
	Needed_RTSlots.push_back(Color0_SLOT);
	

	GFX_API::Framebuffer::RT_SLOT Depth_SLOT;
	Depth_SLOT.ATTACHMENT_TYPE = GFX_API::RT_ATTACHMENTs::TEXTURE_ATTACHMENT_DEPTH;
	Depth_SLOT.RT_OPERATIONTYPE = GFX_API::OPERATION_TYPE::WRITE_ONLY;
	Depth_SLOT.RT_READTYPE = GFX_API::RT_READSTATE::CLEAR;
	Depth_SLOT.CLEAR_COLOR = vec3(1);						//Reversed Depth
	//These should be set by RenderGraph!
	Depth_SLOT.HEIGTH = 0; Depth_SLOT.WIDTH = 0; Depth_SLOT.RT_ID = 0;
	Needed_RTSlots.push_back(Depth_SLOT);
}

void Main_DrawPass::RenderGraph_SetupPhase(vector<GFX_API::Framebuffer::RT_SLOT>& RTs) {
	Is_SetupPhase_Called = true;

	FRAMEBUFFER = GFXContentManager->Create_Framebuffer();

	GFXContentManager->Attach_RenderTarget_toFramebuffer(&RTs[0], GFX_API::RT_ATTACHMENTs::TEXTURE_ATTACHMENT_COLOR0, FRAMEBUFFER);
	GFXContentManager->Attach_RenderTarget_toFramebuffer(&RTs[1], GFX_API::RT_ATTACHMENTs::TEXTURE_ATTACHMENT_DEPTH, FRAMEBUFFER);

	//You don't need to check if attachments are succesful, if they fail then application's not gonna reach here!
	TuranAPI::LOG_STATUS("G-Buffer Draw Pass Creation is done!");
}
void Main_DrawPass::ResourceUpdatePhase() {
	if (!Is_SetupPhase_Called) {
		TuranAPI::LOG_CRASHING("You should call RenderGraph_SetupPhase first!");
	}
	TuranAPI::LOG_NOTCODED("You forgot to code Update_Resources() of G_BUFFER_DrawPass!", false);
}
void Main_DrawPass::Execute() {
	if (!Is_SetupPhase_Called) {
		TuranAPI::LOG_CRASHING("You should call RenderGraph_SetupPhase first!");
	}
	TuranAPI::LOG_STATUS("G-Buffer Render Loop is started to run!");


	GFXRENDERER->Bind_Framebuffer(FRAMEBUFFER);

	mat4 proj_mat = perspective(radians(45.0f), float(1920.0f / 1080.0f), 0.1f, 10000.0f);
	mat4 view_mat;
	view_mat = translate(view_mat, vec3(0, -8,-22));
	mat4 world_transform;

	//Status Report!
	{
		string Before_RenderStatus;
		Before_RenderStatus.append("Mesh number that will be rendered: ");
		Before_RenderStatus.append(std::to_string(DrawCallBuffer.size()).c_str());
		TuranAPI::LOG_STATUS(Before_RenderStatus);
	}

	const GFX_API::DrawCall* DrawCall = nullptr;
	for (unsigned int drawcall_index = 0; drawcall_index < DrawCallBuffer.size(); drawcall_index++) {
		DrawCall = &RG_DrawCallBuffer[DrawCallBuffer[drawcall_index]];

		GFXRENDERER->Bind_MatInstance(DrawCall->ShaderInstance_ID);
		GFXRENDERER->Draw(DrawCall->MeshBuffer_ID);
	}
	TuranAPI::LOG_STATUS("G-Buffer Render Loop is finished!");
}
