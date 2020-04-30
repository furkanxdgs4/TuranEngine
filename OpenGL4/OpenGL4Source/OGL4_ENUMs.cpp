#include "OGL4_ENUMs.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

using namespace TuranAPI;
using namespace GFX_API;

namespace OpenGL4 {
	unsigned int Find_Texture_Dimension(TEXTURE_DIMENSIONs dimension) {
		switch (dimension) {
		case TEXTURE_DIMENSIONs::TEXTURE_2D:
			return GL_TEXTURE_2D;
		default:
			LOG_CRASHING("Intended Texture Dimension isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}

	unsigned int Find_Texture_Format(TEXTURE_CHANNELs channels) {
		switch (channels) {
		case TEXTURE_CHANNELs::API_TEXTURE_RGB:
			return GL_RGB;
		case TEXTURE_CHANNELs::API_TEXTURE_RGBA:
			return GL_RGBA;
		default:
			LOG_CRASHING("Intended Texture Format isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}

	unsigned int Find_Texture_Channel_Type(TEXTURE_CHANNELs channel_type) {
		switch (channel_type) {
		case TEXTURE_CHANNELs::API_TEXTURE_RGB:
			return GL_RGB;
		case TEXTURE_CHANNELs::API_TEXTURE_RGBA:
			return GL_RGBA;
		default:
			LOG_CRASHING("Intended texture channel type isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}

	unsigned int Find_Texture_Value_Type(UNIFORMTYPE value_type) {
		switch (value_type) {
		case UNIFORMTYPE::VAR_UBYTE8:
			return GL_UNSIGNED_BYTE;
		case UNIFORMTYPE::VAR_BYTE8:
			return GL_BYTE;
		case UNIFORMTYPE::VAR_UINT32:
			return GL_UNSIGNED_INT;
		case UNIFORMTYPE::VAR_INT32:
			return GL_INT;
		case UNIFORMTYPE::VAR_FLOAT32:
			return GL_FLOAT;
		default:
			LOG_CRASHING("Intended Texture Value Type isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}

	unsigned int Find_Texture_Wrapping(TEXTURE_WRAPPING wrapping) {
		switch (wrapping) {
		case TEXTURE_WRAPPING::API_TEXTURE_REPEAT:
			return GL_REPEAT;
		case TEXTURE_WRAPPING::API_TEXTURE_MIRRORED_REPEAT:
			return GL_MIRRORED_REPEAT;
		case TEXTURE_WRAPPING::API_TEXTURE_CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		default:
			LOG_CRASHING("Intended Wrapping Type isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}

	unsigned int Find_Texture_Mipmap_Filtering(TEXTURE_MIPMAPFILTER mipmap_filter) {
		switch (mipmap_filter) {
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_1MIP:
			return GL_LINEAR;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_2MIP:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_1MIP:
			return GL_NEAREST;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_2MIP:
			return GL_NEAREST_MIPMAP_LINEAR;
		default:
			LOG_CRASHING("Intended Mipmap Filtering Type isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}
	unsigned int Find_ShaderStage(GFX_API::SHADER_STAGE shader_stage) {
		switch (shader_stage) {
		case GFX_API::SHADER_STAGE::VERTEXSTAGE:
			return GL_VERTEX_SHADER;
		case GFX_API::SHADER_STAGE::FRAGMENTSTAGE:
			return GL_FRAGMENT_SHADER;
		case GFX_API::SHADER_STAGE::COMPUTE:
			return GL_COMPUTE_SHADER;
		default:
			LOG_CRASHING("Intended Shader Stage Type isn't supported by OpenGL4 for now!");
			return NULL;
		}
	}


	unsigned int Find_RenderTarget_Format_Type(TEXTURE_TYPEs texture_format) {
		switch (texture_format) {
		case TEXTURE_TYPEs::COLORTEXTURE:
			return GL_RGBA;
		case TEXTURE_TYPEs::DEPTHTEXTURE:
			return GL_DEPTH_COMPONENT;
		case TEXTURE_TYPEs::DEPTHSTENCIL:
			return GL_DEPTH_STENCIL;
		default:
			LOG_CRASHING("Intended Render Target format isn't supported by OpenGL4 for now!", true);
			return NULL;
		}
	}
	unsigned int Find_RenderTarget_AttachmentType(RT_ATTACHMENTs attachment_type) {
		switch (attachment_type) {
		case RT_ATTACHMENTs::TEXTURE_ATTACHMENT_COLOR0:
			return GL_COLOR_ATTACHMENT0;
		case RT_ATTACHMENTs::TEXTURE_ATTACHMENT_DEPTH:
			return GL_DEPTH_ATTACHMENT;
		case RT_ATTACHMENTs::TEXTURE_ATTACHMENT_DEPTHSTENCIL:
			return GL_DEPTH_STENCIL_ATTACHMENT;
		default:
			LOG_CRASHING("Intended Render Target attachment type isn't supported by OpenGL4 for now!", true);
			return NULL;
		}
	}
	//INPUT HANDLING!

/*
KEYBOARD_KEYs ENGINE Convert_Key_to_Engine(int GLFW_Key) {
	switch (GLFW_Key) {
	case GLFW_KEY_W:
		return KEYBOARD_W;
	case GLFW_KEY_S:
		return KEYBOARD_S;
	case GLFW_KEY_A:
		return KEYBOARD_A;
	case GLFW_KEY_D:
		return KEYBOARD_D;
	case GLFW_KEY_KP_2:
		return KEYBOARD_NP_2;
	case GLFW_KEY_KP_4:
		return KEYBOARD_NP_4;
	case GLFW_KEY_KP_6:
		return KEYBOARD_NP_6;
	case GLFW_KEY_KP_8:
		return KEYBOARD_NP_8;
	case GLFW_KEY_C:
		return KEYBOARD_C;

	default:
		"Error: Intended key isn't supported, returns KEYBOARD_A!\n";
		return KEYBOARD_A;
	}
}

int ENGINE Convert_Key_to_GLFW_Key(KEYBOARD_KEYs Keyboard_Key) {
	switch (Keyboard_Key) {
	case KEYBOARD_W:
		return GLFW_KEY_W;
	case KEYBOARD_S:
		return GLFW_KEY_S;
	case KEYBOARD_A:
		return GLFW_KEY_A;
	case KEYBOARD_D:
		return GLFW_KEY_D;
	case KEYBOARD_NP_2:
		return GLFW_KEY_KP_2;
	case KEYBOARD_NP_4:
		return GLFW_KEY_KP_4;
	case KEYBOARD_NP_6:
		return GLFW_KEY_KP_6;
	case KEYBOARD_NP_8:
		return GLFW_KEY_KP_8;
	case KEYBOARD_C:
		return GLFW_KEY_C;

	default:
		cout << "Error: Intended key isn't supported, return GLFW_KEY_A!\n";
		return GLFW_KEY_A;
	}
}


int ENGINE Convert_MouseButton_to_GLFW_Key(MOUSE_BUTTONs Mouse_Button) {
	switch (Mouse_Button) {
	case MOUSE_LEFT_CLICK:
		return GLFW_MOUSE_BUTTON_LEFT;
	case MOUSE_RIGHT_CLICK:
		return GLFW_MOUSE_BUTTON_RIGHT;
	case MOUSE_WHEEL_CLICK:
		return GLFW_MOUSE_BUTTON_MIDDLE;

	default:
		cout << "Error: Intended mouse button isn't supported, return GLFW_MOUSE_BUTTON_1!\n";
		return GLFW_MOUSE_BUTTON_1;
	}
}

*/



	unsigned int Find_GFX_DepthTest_Mode(DEPTH_TESTs depth_test) {
		switch (depth_test) {
		case DEPTH_TESTs::DEPTH_TEST_ALWAYS:
			return GL_ALWAYS;
		case DEPTH_TESTs::DEPTH_TEST_GEQUAL:
			return GL_GEQUAL;
		case DEPTH_TESTs::DEPTH_TEST_GREATER:
			return GL_GREATER;
		case DEPTH_TESTs::DEPTH_TEST_LEQUAL:
			return GL_LEQUAL;
		case DEPTH_TESTs::DEPTH_TEST_LESS:
			return GL_LESS;
		case DEPTH_TESTs::DEPTH_TEST_NEVER:
			return GL_NEVER;
		default:
			LOG_NOTCODED("Intended Depth Test Mode can't be found! GL_NEVER is returned!", true);
			return GL_NEVER;
		}
	}
	
	void Check_GL_Errors(const char* status) {
		int error = glGetError();
		//If there is a error, first print the status!
		if (error != NULL) {
			TuranAPI::LOG_ERROR(status);
		}
		else {
			return;
		}

		//Print the error!
		if (error == GL_INVALID_OPERATION) {
			TuranAPI::LOG_ERROR("GL_INVALID_OPERATION!");
		}
		if (error == GL_INVALID_ENUM) {
			TuranAPI::LOG_ERROR("GL_INVALID_ENUM!");
		}
		if (error == GL_INVALID_VALUE) {
			TuranAPI::LOG_ERROR("GL_INVALID_VALUE!");
		}
		SLEEP_THREAD(10);
	}

	void Check_ActiveFramebuffer_Status(const char* Framebuffer_Name) {
		String log;
		int fb_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fb_status == GL_FRAMEBUFFER_COMPLETE) {
			log.append("Framebuffer is completed, Name: ");
			log.append(Framebuffer_Name);
			TuranAPI::LOG_STATUS(log);
		}
		else if (fb_status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			log.append("Framebuffer has incomplete attachment, Name: ");
			log.append(Framebuffer_Name);
			TuranAPI::LOG_CRASHING(log);
		}
		else if (fb_status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			log.append("Framebuffer has incomplete missing attachment, Name: ");
			log.append(Framebuffer_Name);
			TuranAPI::LOG_CRASHING(log);
		}
		else if (fb_status == GL_FRAMEBUFFER_UNSUPPORTED) {
			log.append("Framebuffer has unsupported type of attachment, Name: ");
			log.append(Framebuffer_Name);
			TuranAPI::LOG_CRASHING(log);
		}
	}
}
