#pragma once
#include "OpenGL4Source/OGL4_ENUMs.h"

namespace OpenGL4 {
	class OGL4_API OpenGL4_Renderer : public GFX_API::Renderer {
	protected:
		friend class OpenGL4_Core;

		virtual void Bind_Framebuffer(unsigned int FB_ID) override;
		virtual void Bind_MatInstance(unsigned int MatInstance_ID) override;
		virtual void Draw(unsigned int MeshBuffer_ID) override;
		//SOME OF THE BELOW FUNCTIONS SHOULD GO SOME OTHER SYSTEMS BECAUSE RENDERER WILL BE ONLY RESPONSIBLE FOR CONVERTING RENDERGRAPH'S CALLS TO REAL GL CALLS



		//RENDERER FUNCTIONs

	//Bindings
		/*
		virtual void Bind_Framebuffer(GFX_Framebuffer* FRAMEBUFFER) override;
		virtual void Attach_RenderTargets_to_Framebuffer(GFX_Framebuffer* FRAMEBUFFER) override;
		virtual void Bind_Material_Instance(TAPIFILESYSTEM::Material_Instance* Material_Instance) override;
		virtual void Show_RenderTarget_onWindow(GFX_WINDOW* WINDOW_to_Display, GFX_Render_Target* RenderTarget_to_Show) override;
		//Resource Creation

		virtual GFX_Framebuffer* Create_Framebuffer(unsigned int width, unsigned int height) override;
		virtual void Create_RenderTarget(GFX_Framebuffer* FRAMEBUFFER, unsigned int width, unsigned int height, TuranAPI_ENUMs dimension, GFX_ENUM format, GFX_ENUM attachment, TuranAPI_ENUMs value_type)  override;
		virtual void Compile_MaterialType(string* compilation_status, const string* vertex_source, const string* fragment_source, unsigned int* program_id = nullptr, unsigned int* vertex_id = nullptr, unsigned int* fragment_id = nullptr)  override;
		//Resource Clear/Destroy (Add here shader, model, texture destructions!)

		virtual void Clear_RenderTarget(GFX_Framebuffer* FRAMEBUFFER, GFX_ENUM ATTACHMENT, vec3 CLEAR_COLOR) override;
		//Resource Setting (Add here texture type changes etc)

		virtual void Change_Framebuffer_Resolution(GFX_Framebuffer* FRAMEBUFFER, unsigned int width, unsigned int height) override;
		//Render State

		virtual void Set_Depth_Test(GFX_ENUM DEPTH_TEST_MODE, GFX_ENUM DEPTH_BUFFER_MODE) override;
		virtual void Check_ActiveFramebuffer_Status(string name_of_framebuffer) override;
		//Sending data to GPU

		virtual void Send_Textures_to_GPU(vector<TAPIFILESYSTEM::Texture_Resource*>* Textures) override;
		virtual void Send_StaticModelDatas_to_GPU(vector<TAPIFILESYSTEM::Static_Model_Data*>* StaticModel_Datas) override;
		//Draw Polygon

		//Draw multiple objects that use index buffers! No shader program changes!
		virtual void Draw_Indexed_Objects(vector<void*> Vertex_Attributes, vector<unsigned int> Indice_Numbers) override;
		*/
	};
}