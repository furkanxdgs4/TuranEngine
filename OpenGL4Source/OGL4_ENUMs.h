#pragma once
#include "TuranAPI/API_ENUMs.h"
#include "GFXSource/GFX_Core.h"
#include "OpenGL4_Includes.h"


namespace OpenGL4 {
	unsigned int OGL4_API Find_Texture_Dimension(GFX_API::TEXTURE_DIMENSIONs dimension);
	unsigned int OGL4_API Find_Texture_Channel_Type(GFX_API::TEXTURE_CHANNELs channel_type);
	unsigned int OGL4_API Find_Texture_Value_Type(GFX_API::UNIFORMTYPE value_type);
	unsigned int OGL4_API Find_Texture_Wrapping(GFX_API::TEXTURE_WRAPPING wrapping);
	unsigned int OGL4_API Find_Texture_Mipmap_Filtering(GFX_API::TEXTURE_MIPMAPFILTER mipmap_filter);

	unsigned int OGL4_API Find_ShaderStage(GFX_API::SHADER_STAGE shader_stage);

	unsigned int OGL4_API Find_RenderTarget_Format_Type(GFX_API::TEXTURE_TYPEs texture_format);
	unsigned int OGL4_API Find_RenderTarget_AttachmentType(GFX_API::RT_ATTACHMENTs texture_attachment);

	//KEYBOARD_KEYs OGL3_API Convert_Key_to_Engine(int GLFW_Key);
	//int OGL3_API Convert_Key_to_GLFW_Key(KEYBOARD_KEYs Keyboard_Key);
	//int OGL3_API Convert_MouseButton_to_GLFW_Key(MOUSE_BUTTONs Mouse_Button);

	unsigned int OGL4_API Find_GFX_DepthTest_Mode(GFX_API::DEPTH_TESTs depth_test);


	void OGL4_API Check_GL_Errors(const char* status);
	void Check_ActiveFramebuffer_Status(const char* Framebuffer_Name);
}
