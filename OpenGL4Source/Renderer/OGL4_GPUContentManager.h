#pragma once
#include "OpenGL4Source/OpenGL4_Includes.h"
#include "GFXSource/Renderer/GPU_ContentManager.h"

namespace OpenGL4 {

	struct OGL4_API OGL4_MESH {
		unsigned int VAO, VBO, EBO;
	};


	class OGL4_API GPU_ContentManager : public GFX_API::GPU_ContentManager {
	public:
		GPU_ContentManager();
		virtual void Unload_AllResources();

		//Return MeshBufferID to use in Draw Calls
		virtual unsigned int Upload_MeshBuffer(const GFX_API::VertexAttributeLayout& attributelayout, const void* vertex_data, unsigned int data_size, unsigned int vertex_count) override;
		//When you call this function, Draw Calls that uses this ID may draw another Mesh or crash
		virtual void Unload_MeshBuffer(unsigned int MeshBuffer_ID) override;


		virtual void Upload_Texture(GFX_API::Texture_Resource* TEXTURE_ASSET, unsigned int Asset_ID, bool Generate_Mipmap) override;
		virtual void Unload_Texture(unsigned int ASSET_ID);


		virtual void Compile_ShaderSource(GFX_API::ShaderSource_Resource* SHADER, unsigned int Asset_ID, string* compilation_status) override;
		virtual void Delete_ShaderSource(unsigned int ASSET_ID) override;
		virtual void Link_MaterialType(GFX_API::Material_Type* MATTYPE_ASSET, string* compilation_status) override;
		virtual void Delete_MaterialType(unsigned int Asset_ID) override;


		virtual unsigned int Create_RenderTarget(unsigned int WIDTH, unsigned int HEIGTH, GFX_API::TEXTURE_DIMENSIONs DIMENSION,
			GFX_API::TEXTURE_TYPEs FORMAT, GFX_API::UNIFORMTYPE FORMAT_VALUETYPE, bool Usable_as_Texture) override;
		virtual void Delete_RenderTarget(unsigned int RT_ID) override;


		virtual unsigned int Create_Framebuffer() override;
		virtual void Attach_RenderTarget_toFramebuffer(const GFX_API::Framebuffer::RT_SLOT* RT_SLOT, GFX_API::RT_ATTACHMENTs ATTACHMENT_TYPE, unsigned int FB_ID) override;
		virtual void Delete_Framebuffer(unsigned int Framebuffer_ID) override;
	};


}