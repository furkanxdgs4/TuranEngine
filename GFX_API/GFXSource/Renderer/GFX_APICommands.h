#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_Display.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

namespace GFX_API {
	//API Command class
	class GFXAPI APICommander {
	public:
		static APICommander* SELF;

		//Clear color of the default render target of window!
		virtual void Clear_WindowBuffer_Immediately(WINDOW* window, vec3 Clear_Color) = 0;
		//Do the Swap between a window's Back-Front buffers
		virtual void Swap_WindowBuffers_Immediately(WINDOW* window) = 0;
		//Send a resource to the GPU
		virtual void Send_Resource_toGPU_Immediately(TuranAPI::Resource_Type* RESOURCE) = 0;
	};

	namespace APICommand {
		//Write here recordable API Commands such as Draw Calls, Binding as storage structs
	}
}

/*
//Bindings

virtual void Bind_Framebuffer(GFX_Framebuffer* FRAMEBUFFER) = 0;
virtual void Attach_RenderTargets_to_Framebuffer(GFX_Framebuffer* FRAMEBUFFER) = 0;
virtual void Bind_Material_Instance(TAPIFILESYSTEM::Material_Instance* Material_Instance) = 0;
virtual void Show_RenderTarget_onWindow(GFX_WINDOW* WINDOW_to_Display, GFX_Render_Target* RenderTarget_to_Show) = 0;
//Resource Creation

virtual GFX_Framebuffer* Create_Framebuffer(unsigned int width, unsigned int height) = 0;
virtual void Create_RenderTarget(GFX_Framebuffer* FRAMEBUFFER, unsigned int width, unsigned int height, TuranAPI_ENUMs dimension, GFX_ENUM format, GFX_ENUM attachment, TuranAPI_ENUMs value_type) = 0;
virtual void Compile_MaterialType(string* compilation_status, const string* vertex_source, const string* fragment_source, unsigned int* program_id = nullptr, unsigned int* vertex_id = nullptr, unsigned int* fragment_id = nullptr) = 0;
static void Compile_All_MaterialTypes();
//Resource Clear/Destroy (Add here shader, model, texture destructions!)

virtual void Clear_RenderTarget(GFX_Framebuffer* FRAMEBUFFER, GFX_ENUM ATTACHMENT, vec3 CLEAR_COLOR) = 0;
//Resource Setting (Add here texture type changes etc)

virtual void Change_Framebuffer_Resolution(GFX_Framebuffer* FRAMEBUFFER, unsigned int width, unsigned int height) = 0;
//Render State

virtual void Set_Depth_Test(GFX_ENUM DEPTH_TEST_MODE, GFX_ENUM DEPTH_BUFFER_MODE) = 0;
virtual void Check_ActiveFramebuffer_Status(string name_of_framebuffer) = 0;
//Sending data to GPU

virtual void Send_Textures_to_GPU(vector<TAPIFILESYSTEM::Texture_Resource*>* Textures) = 0;
virtual void Send_StaticModelDatas_to_GPU(vector<TAPIFILESYSTEM::Static_Model_Data*>* StaticModel_Datas) = 0;
//Draw Polygon

virtual void Draw_Indexed_Objects(vector<void*> Vertex_Attributes, vector<unsigned int> Indice_Numbers) = 0;
*/