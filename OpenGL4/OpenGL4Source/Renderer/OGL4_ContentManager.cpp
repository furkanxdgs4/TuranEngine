#include "OGL4_GPUContentManager.h"
#include "OpenGL4Source/OGL4_ENUMs.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>

namespace OpenGL4 {
	//If there is at least one Continuous Attribute before, that means there should be a Start_Offset in one of the them!
	size_t OGL4_API Get_WherePreviousAttribute_Ends(const GFX_API::VertexAttributeLayout* attributelayout, size_t vertex_count, unsigned int currentattribute_index) {
		if (currentattribute_index == 0) {
			return 0;
		}
		GFX_API::VertexAttribute* previous_attribute = attributelayout->Attributes[currentattribute_index - 1];
		if (previous_attribute->Start_Offset == 0) {
			return Get_WherePreviousAttribute_Ends(attributelayout, vertex_count, currentattribute_index - 1) + (GFX_API::Get_UNIFORMTYPEs_SIZEinbytes(previous_attribute->DATATYPE) * vertex_count);
		}
		else {
			return previous_attribute->Start_Offset + (GFX_API::Get_UNIFORMTYPEs_SIZEinbytes(previous_attribute->DATATYPE) * vertex_count);
		}
	}
	void OGL4_API Set_VertexAttribPointer(const GFX_API::VertexAttribute* attribute, size_t Start_Offset) {
		switch (attribute->DATATYPE)
		{
		case GFX_API::UNIFORMTYPE::VAR_FLOAT32:
			glVertexAttribPointer(attribute->Index, 1, GL_FLOAT, GL_FALSE, sizeof(float32), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_INT32:
			glVertexAttribPointer(attribute->Index, 1, GL_INT, GL_FALSE, sizeof(int), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_UINT32:
			glVertexAttribPointer(attribute->Index, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned int), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_BYTE8:
			glVertexAttribPointer(attribute->Index, 1, GL_BYTE, GL_FALSE, sizeof(char), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_UBYTE8:
			glVertexAttribPointer(attribute->Index, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(unsigned char), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_VEC2:
			glVertexAttribPointer(attribute->Index, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_VEC3:
			glVertexAttribPointer(attribute->Index, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_VEC4:
			glVertexAttribPointer(attribute->Index, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (void*)(Start_Offset));
			break;
		case GFX_API::UNIFORMTYPE::VAR_MAT4x4:
			glVertexAttribPointer(attribute->Index, 16, GL_FLOAT, GL_FALSE, sizeof(mat4x4), (void*)(Start_Offset));
			break;
		default:
			TuranAPI::LOG_CRASHING("Uniform Type isn't supported to set to Vertex Attribute!");
			break;
		}
	}

	GPU_ContentManager::GPU_ContentManager() : MESHBUFFERs(LASTUSEDALLOCATOR, 100, 1000), 
		TEXTUREs(LASTUSEDALLOCATOR, 100, 1000), SHADERSOURCEs(LASTUSEDALLOCATOR, 100, 1000), SHADERPROGRAMs(LASTUSEDALLOCATOR, 100, 1000), RTs(LASTUSEDALLOCATOR, 10, 20), FBs(LASTUSEDALLOCATOR, 10, 30){

	}


	unsigned int GPU_ContentManager::Upload_MeshBuffer(const GFX_API::VertexAttributeLayout& attributelayout, const void* vertex_data, unsigned int data_size, unsigned int vertex_count) {
		if (!attributelayout.Does_BufferFits_Layout(vertex_data, data_size, vertex_count)) {
			return 0;
		}
		
		OGL4_MESH MESH;
		glGenVertexArrays(1, &MESH.VAO);
		glGenBuffers(1, &MESH.VBO);
		glGenBuffers(1, &MESH.EBO);
		glBindVertexArray(MESH.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, MESH.VBO);
		glBufferData(GL_ARRAY_BUFFER, data_size, vertex_data, GL_STATIC_DRAW);

		for (unsigned int attribute_index = 0; attribute_index < attributelayout.Attributes.size(); attribute_index++) {
			GFX_API::VertexAttribute* attribute = attributelayout.Attributes[attribute_index];
			//Reporting status
			{
				String Status = "Uploading Vertex Attribute: ";
				Status.append(attribute->AttributeName);
				TuranAPI::LOG_STATUS(Status);
			}


			glEnableVertexAttribArray(attribute->Index);
			size_t Start_Offset = 0;
			if (attribute->Start_Offset == 0) {
				Start_Offset = Get_WherePreviousAttribute_Ends(&attributelayout, vertex_count, attribute_index);
				Set_VertexAttribPointer(attribute, Start_Offset);
			}
			else {
				Set_VertexAttribPointer(attribute, attribute->Start_Offset);
			}
		}
		MESH.BUFFER_ID = Create_MeshBufferID();
		MESHBUFFERs.push_back(MESH);
		glBindVertexArray(0);
		return MESH.BUFFER_ID;
	}
	void GPU_ContentManager::Unload_MeshBuffer(unsigned int MeshBuffer_ID) {
		OGL4_MESH* MESHBUFFER = Find_MeshBuffer_byBufferID(MeshBuffer_ID);
		if (MESHBUFFER) {
			glDeleteVertexArrays(1, &MESHBUFFER->VAO);
			glDeleteBuffers(1, &MESHBUFFER->VBO);
			glDeleteBuffers(1, &MESHBUFFER->EBO);
			return;
		}

		TuranAPI::LOG_ERROR("Unload has failed because intended Mesh Buffer isn't found in OpenGL::GPU_ContentManager!");
	}

	void GPU_ContentManager::Unload_AllResources() {
		TuranAPI::LOG_NOTCODED("OpenGL::GPU_ContentManager->Unload_AllResources() isn't coded yet!", false);
	}

	void GPU_ContentManager::Upload_Texture(GFX_API::Texture_Resource* ASSET, bool Generate_Mipmap) {
		unsigned int TEXTURE_DIMENSION = Find_Texture_Dimension(ASSET->Properties.DIMENSION);
		unsigned int TEXTURE_CHANNELs = Find_Texture_Channel_Type(ASSET->Properties.CHANNEL_TYPE);
		unsigned int TEXTURE_WRAPPING = Find_Texture_Wrapping(ASSET->Properties.WRAPPING);
		unsigned int TEXTURE_MIPMAPFILTER = Find_Texture_Mipmap_Filtering(ASSET->Properties.MIPMAP_FILTERING);
		unsigned int TEXTURE_VALUETYPE = Find_Texture_Value_Type(ASSET->Properties.VALUE_TYPE);


		unsigned int TEXTUREID = 0;
		glGenTextures(1, &TEXTUREID);
		glBindTexture(TEXTURE_DIMENSION, TEXTUREID);


		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_WRAP_S, TEXTURE_WRAPPING);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_WRAP_T, TEXTURE_WRAPPING);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_MIN_FILTER, TEXTURE_MIPMAPFILTER);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_MAG_FILTER, TEXTURE_MIPMAPFILTER);

		glTexImage2D(TEXTURE_DIMENSION, 0, TEXTURE_CHANNELs, ASSET->WIDTH, ASSET->HEIGHT, 0, TEXTURE_CHANNELs, TEXTURE_VALUETYPE, ASSET->DATA);
		if (Generate_Mipmap) {
			glGenerateMipmap(TEXTURE_DIMENSION);
		}

		TuranAPI::LOG_NOTCODED("Texture_Resource should be able to store its own Mipmaps in Disk and we should upload them one by one here!\n", false);

		OGL4_TEXTURE texture;
		texture.ASSET_ID = ASSET->ID;
		texture.OGL_ID = TEXTUREID;
		TEXTUREs.push_back(texture);
	}
	void GPU_ContentManager::Unload_Texture(unsigned int TEXTURE_ID) {
		glDeleteTextures(1, &TEXTUREs[Find_TextureGLID_byAssetID(TEXTURE_ID)].OGL_ID);
	}
	void GPU_ContentManager::Link_MaterialType(GFX_API::Material_Type_Resource* MATTYPE_ASSET, String* compilation_status) {
		//Link and return the Shader Program!
		unsigned int SHADERPROGRAM = glCreateProgram();

		//Link Vertex and Fragment Shader to Shader Program and set ID
		glAttachShader(SHADERPROGRAM, MATTYPE_ASSET->VERTEXSOURCE_ID);
		glAttachShader(SHADERPROGRAM, MATTYPE_ASSET->FRAGMENTSOURCE_ID);
		glLinkProgram(SHADERPROGRAM);

		//Check linking issues
		int link_success;
		char link_infolog[512];
		glGetProgramiv(SHADERPROGRAM, GL_LINK_STATUS, &link_success);
		if (!link_success) {
			glGetProgramInfoLog(SHADERPROGRAM, 512, NULL, link_infolog);
			compilation_status->append(link_infolog);
			TuranAPI::LOG_CRASHING(link_infolog);
			return;
		}
		compilation_status->append("Succesfully linked!");
	}
	void GPU_ContentManager::Delete_MaterialType(unsigned int Asset_ID) {
		glDeleteProgram(Find_ShaderProgramID_byAssetID(Asset_ID));
	}
	void GPU_ContentManager::Compile_ShaderSource(GFX_API::ShaderSource* SHADER, String* compilation_status) {
		unsigned int STAGE = Find_ShaderStage(SHADER->STAGE);

		unsigned int SHADER_o = glCreateShader(STAGE);
		auto x = SHADER->SOURCE_CODE.c_str();
		glShaderSource(SHADER_o, 1, &x, NULL);
		glCompileShader(SHADER_o);

		//Check compile issues!
		int success;
		char vert_infolog[512];
		glGetShaderiv(SHADER_o, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(SHADER_o, 512, NULL, vert_infolog);
			compilation_status->append(vert_infolog);
			TuranAPI::LOG_ERROR(compilation_status->c_str());
			return;
		}
		OGL4_SHADERSOURCE shadersource;
		shadersource.ASSET_ID = SHADER->ID;
		shadersource.OGL_ID = SHADER_o;
		SHADERSOURCEs.push_back(shadersource);
		compilation_status->append("Succesfully compiled!");
	}
	void GPU_ContentManager::Delete_ShaderSource(unsigned int Asset_ID) {
		glDeleteShader(Find_ShaderSourceGLID_byAssetID(Asset_ID));
	}


	unsigned int GPU_ContentManager::Create_RenderTarget(unsigned int WIDTH, unsigned int HEIGTH, GFX_API::TEXTURE_DIMENSIONs DIMENSION,
		GFX_API::TEXTURE_TYPEs FORMAT, GFX_API::UNIFORMTYPE VALUETYPE, bool Usable_as_Texture) {
		OGL4_RT RT;
		RT.RT_Properties.WIDTH = WIDTH; RT.RT_Properties.HEIGHT = HEIGTH; RT.RT_Properties.DIMENSION = DIMENSION; RT.RT_Properties.FORMAT = FORMAT;
		RT.RT_Properties.FORMAT_VALUETYPE = VALUETYPE; RT.RT_Properties.Usable_as_Texture = Usable_as_Texture;

		//Create a Texture!
		if (Usable_as_Texture) {
			unsigned int GL_FORMAT = Find_RenderTarget_Format_Type(FORMAT);
			unsigned int GL_DIMENSION = Find_Texture_Dimension(DIMENSION);
			unsigned int GL_VALUE_TYPE = Find_Texture_Value_Type(VALUETYPE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenTextures(1, &RT.OGL_ID);
			glBindTexture(GL_TEXTURE_2D, RT.OGL_ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_FORMAT, WIDTH, HEIGTH, 0, GL_FORMAT, GL_VALUE_TYPE, NULL);
			{
				String status;
				status.append("Created a Render Target's OpenGL ID: ");
				status.append(std::to_string(RT.OGL_ID).c_str());
				TuranAPI::LOG_STATUS(status);
			}

			Check_GL_Errors("Created a Render Texture!");
		}
		//Create a Render Buffer
		else {
			TuranAPI::LOG_NOTCODED("Render Buffer creation isn't supported, you should set Usable_as_Texture to true!\n", true);
			return 0;
		}

		RT.RT_Properties.ID = Create_RenderTargetID();
		RTs.push_back(RT);
		return RT.RT_Properties.ID;
	}
	const GFX_API::RenderTarget& GPU_ContentManager::Find_RenderTargetProperties_byID(unsigned int RT_ID) {
		for (unsigned int i = 0; i < RTs.size(); i++) {
			GFX_API::RenderTarget& RT = RTs[i].RT_Properties;
			if (RT.ID == RT_ID) {
				return RT;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Render Target isn't found in OpenGL::GPU_ContentManager!");
	}
	void GPU_ContentManager::Delete_RenderTarget(unsigned int RT_ID) {
		for (unsigned int i = 0; i < RTs.size(); i++) {
			OGL4_RT& RT = RTs[i];
			if (RT.RT_Properties.ID == RT_ID) {
				if (RT.RT_Properties.Usable_as_Texture) {
					glDeleteTextures(1, &RT.OGL_ID);
				}
				else {
					glDeleteRenderbuffers(1, &RT.OGL_ID);
				}
				RTs.erase(i);
			}
		}
	}

	unsigned int GPU_ContentManager::Create_Framebuffer() {
		OGL4_FB FB;
		
		glGenFramebuffers(1, &FB.OGL_ID);
		Check_GL_Errors("After creating a Framebuffer!");
		FB.FB_Properties.ID = Create_FrameBufferID();
		FBs.push_back(FB);
		return FB.FB_Properties.ID;
	}

	void GPU_ContentManager::Attach_RenderTarget_toFramebuffer(unsigned int RT_ID, GFX_API::RT_ATTACHMENTs ATTACHMENT_TYPE, unsigned int FB_ID) {
		OGL4_FB& FB = Find_OGLFB_byGFXID(FB_ID);

		const GFX_API::RenderTarget& RT = Find_RenderTargetProperties_byID(RT_ID);
		unsigned int RT_GLID = Find_RenderTargetID_byGFXID(RT.ID);

		glBindFramebuffer(GL_FRAMEBUFFER, Find_FrameBufferGLID_byGFXID(FB_ID));
		if (RT.Usable_as_Texture) {
			glBindTexture(GL_TEXTURE_2D, RT_GLID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, Find_RenderTarget_AttachmentType(ATTACHMENT_TYPE), GL_TEXTURE_2D, RT_GLID, 0);
		}
		else {
			TuranAPI::LOG_CRASHING("Creating a Render Buffer for RT isn't supported for now, so you can't bind it to Framebuffer! There is a problem somewhere");
			return;
		}
		Check_ActiveFramebuffer_Status(std::to_string(FB.FB_Properties.ID).c_str());


		GFX_API::Framebuffer::RT_SLOT RT_SLOT;
		RT_SLOT.ATTACHMENT_TYPE = ATTACHMENT_TYPE;
		RT_SLOT.RT_ID = RT_ID;
		FB.FB_Properties.BOUND_RTs.push_back(RT_SLOT);
	}

	void GPU_ContentManager::Delete_Framebuffer(unsigned int Framebuffer_ID) {
		glDeleteFramebuffers(1, &Find_FrameBufferGLID_byGFXID(Framebuffer_ID));
	}















	//HELPER FUNCTIONs

	OGL4_MESH* GPU_ContentManager::Find_MeshBuffer_byBufferID(unsigned int ID) {
		for (unsigned int i = 0; i < MESHBUFFERs.size(); i++) {
			OGL4_MESH* MESHBUFFER = &MESHBUFFERs[i];
			if (MESHBUFFER->BUFFER_ID == ID) {
				return MESHBUFFER;
			}
		}

		TuranAPI::LOG_CRASHING("Intended Mesh Buffer's GL_ID isn't found in OpenGL::GPU_ContentManager!");
		return nullptr;
	}
	unsigned int& GPU_ContentManager::Find_TextureGLID_byAssetID(unsigned int Texture_ID) {
		for (unsigned int i = 0; i < TEXTUREs.size(); i++) {
			OGL4_TEXTURE& TEXTURE = TEXTUREs[i];
			if (TEXTURE.ASSET_ID == Texture_ID) {
				return TEXTURE.OGL_ID;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Texture GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}
	unsigned int& GPU_ContentManager::Find_ShaderSourceGLID_byAssetID(unsigned int ShaderSource_ID) {
		for (unsigned int i = 0; i < SHADERSOURCEs.size(); i++) {
			OGL4_SHADERSOURCE& SHADERSOURCE = SHADERSOURCEs[i];
			if (SHADERSOURCE.ASSET_ID == ShaderSource_ID) {
				return SHADERSOURCE.OGL_ID;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Shader Source GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}
	unsigned int& GPU_ContentManager::Find_ShaderProgramID_byAssetID(unsigned int ShaderProgram_ID) {
		for (unsigned int i = 0; i < SHADERPROGRAMs.size(); i++) {
			OGL4_SHADERPROGRAM& SHADERPROGRAM = SHADERPROGRAMs[i];
			if (SHADERPROGRAM.ASSET_ID == ShaderProgram_ID) {
				return SHADERPROGRAM.OGL_ID;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Shader Program GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}
	unsigned int& GPU_ContentManager::Find_RenderTargetID_byGFXID(unsigned int GFX_RT_ID) {
		for (unsigned int i = 0; i < RTs.size(); i++) {
			OGL4_RT& RT = RTs[i];
			if (RT.RT_Properties.ID == GFX_RT_ID) {
				return RT.OGL_ID;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Render Target GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}
	unsigned int& GPU_ContentManager::Find_FrameBufferGLID_byGFXID(unsigned int GFX_FB_ID) {
		for (unsigned int i = 0; i < FBs.size(); i++) {
			OGL4_FB& FB = FBs[i];
			if (FB.FB_Properties.ID == GFX_FB_ID) {
				return FB.OGL_ID;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Framebuffer GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}




	OGL4_FB& GPU_ContentManager::Find_OGLFB_byGFXID(unsigned int GFX_FB_ID) {
		for (unsigned int i = 0; i < FBs.size(); i++) {
			OGL4_FB& FB = FBs[i];
			if (FB.FB_Properties.ID == GFX_FB_ID) {
				return FB;
			}
		}
		TuranAPI::LOG_CRASHING("Intended Framebuffer GL_ID isn't found in OpenGL::GPU_ContentManager!");
	}
}