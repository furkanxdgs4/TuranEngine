#pragma once
#include "OpenGL4Source/OpenGL4_Includes.h"
#include "GFXSource/Renderer/GFX_APICommands.h"

#include "GFXSource/GFX_Display.h"

namespace OpenGL4 {
	class OGL4_API APICommander : public GFX::APICommander {
	public:

		virtual void Clear_WindowBuffer_Immediately(GFX::WINDOW* window, vec3 Clear_Color) override;
		virtual void Swap_WindowBuffers_Immediately(GFX::WINDOW* window) override;
	};

	namespace APICommand {
		//Write here recordable API Commands such as Draw Calls, Binding as storage structs
	}
}