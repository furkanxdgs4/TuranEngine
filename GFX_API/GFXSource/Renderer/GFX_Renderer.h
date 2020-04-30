#pragma once
#include "GFXSource/GFX_Includes.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"

#include "GFX_Resource.h"
#include "GFX_RenderCommands.h"
#include "GFX_RenderGraph.h"

namespace GFX_API {
	class GFXAPI Renderer {
	protected:

	public:
		friend class GFX_Core;

		Renderer();
		~Renderer();
		void* Renderer_Context;
		Renderer* SELF;


		//RENDERER FUNCTIONs
		virtual void Bind_Framebuffer(unsigned int FB_ID) = 0;
		virtual void Bind_MatInstance(unsigned int MatInstance_ID) = 0;
		virtual void Draw(unsigned int MeshBuffer_ID) = 0;
	};
}