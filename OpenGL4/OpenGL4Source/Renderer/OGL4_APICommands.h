#pragma once
#include "OpenGL4Source/OGL4_ENUMs.h"
#include "GFXSource/Renderer/GFX_APICommands.h"

#include "GFXSource/GFX_Display.h"

namespace OpenGL4 {
	class OGL4_API APICommander : public GFX_API::APICommander {
	public:

		virtual void Clear_WindowBuffer_Immediately(GFX_API::WINDOW* window, vec3 Clear_Color) override;
		virtual void Swap_WindowBuffers_Immediately(GFX_API::WINDOW* window) override;
	};

	namespace APICommand {
		//Write here recordable API Commands such as Draw Calls, Binding as storage structs
	}
}