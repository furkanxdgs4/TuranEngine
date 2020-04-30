#pragma once
#include "OpenGL4Source/OpenGL4_Includes.h"
#include "GFXSource/Renderer/GPU_ContentManager.h"

namespace OpenGL4 {
	struct OGL4_API OGL4_MESH {
		unsigned int BUFFER_ID, VAO, VBO, EBO;
	};
	struct OGL4_API OGL4_SHADERSOURCE {
		unsigned int ASSET_ID, OGL_ID;
	};
	struct OGL4_API OGL4_TEXTURE {
		unsigned int ASSET_ID, OGL_ID;
	};
	struct OGL4_API OGL4_SHADERPROGRAM {
		unsigned int ASSET_ID, OGL_ID;
	};

	struct OGL4_API OGL4_RT {
		unsigned int OGL_ID;
		GFX_API::RenderTarget RT_Properties;
	};
	struct OGL4_API OGL4_FB {
		unsigned int OGL_ID;
		GFX_API::Framebuffer FB_Properties;
	};

	class OGL4_API GPU_ContentManager : public GFX_API::GPU_ContentManager {
	public:
		Vector<OGL4_MESH> MESHBUFFERs;
		Vector<OGL4_TEXTURE> TEXTUREs;
		Vector<OGL4_SHADERSOURCE> SHADERSOURCEs;
		Vector<OGL4_SHADERPROGRAM> SHADERPROGRAMs;

		Vector<OGL4_RT> RTs;
		Vector<OGL4_FB> FBs;


		GPU_ContentManager();
		virtual void Unload_AllResources();

		//Return MeshBufferID to use in Draw Calls
		virtual unsigned int Upload_MeshBuffer(const GFX_API::VertexAttributeLayout& attributelayout, const void* vertex_data, unsigned int data_size, unsigned int vertex_count) override;
		//When you call this function, Draw Calls that uses this ID may draw another Mesh or crash
		virtual void Unload_MeshBuffer(unsigned int MeshBuffer_ID) override;


		virtual void Upload_Texture(GFX_API::Texture_Resource* TEXTURE_ASSET, bool Generate_Mipmap) override;
		virtual void Unload_Texture(unsigned int ASSET_ID);


		virtual void Compile_ShaderSource(GFX_API::ShaderSource* SHADER, String* compilation_status) override;
		virtual void Delete_ShaderSource(unsigned int ASSET_ID) override;
		virtual void Link_MaterialType(GFX_API::Material_Type_Resource* MATTYPE_ASSET, String* compilation_status) override;
		virtual void Delete_MaterialType(unsigned int Asset_ID) override;


		virtual unsigned int Create_RenderTarget(unsigned int WIDTH, unsigned int HEIGTH, GFX_API::TEXTURE_DIMENSIONs DIMENSION,
			GFX_API::TEXTURE_TYPEs FORMAT, GFX_API::UNIFORMTYPE FORMAT_VALUETYPE, bool Usable_as_Texture) override;
		virtual const GFX_API::RenderTarget& Find_RenderTargetProperties_byID(unsigned int RT_ID) override;
		virtual void Delete_RenderTarget(unsigned int RT_ID) override;


		virtual unsigned int Create_Framebuffer() override;
		virtual void Attach_RenderTarget_toFramebuffer(unsigned int RT_ID, GFX_API::RT_ATTACHMENTs ATTACHMENT_TYPE, unsigned int FB_ID) override;
		virtual void Delete_Framebuffer(unsigned int Framebuffer_ID) override;


			//HELPER FUNCTIONs
		unsigned int& Find_TextureGLID_byAssetID(unsigned int TextureAsset_ID);
		unsigned int& Find_ShaderSourceGLID_byAssetID(unsigned int ShaderSource_ID);
		unsigned int& Find_ShaderProgramID_byAssetID(unsigned int MatTypeAsset_ID);
		unsigned int& Find_RenderTargetID_byGFXID(unsigned int GFX_RT_ID);
		unsigned int& Find_FrameBufferGLID_byGFXID(unsigned int GFX_FB_ID);

		OGL4_MESH* Find_MeshBuffer_byBufferID(unsigned int MeshBuffer_ID);
		OGL4_FB& Find_OGLFB_byGFXID(unsigned int GFX_FB_ID);
	};


}