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

	GPU_ContentManager::GPU_ContentManager(){

	}


	unsigned int GPU_ContentManager::Upload_MeshBuffer(const GFX_API::VertexAttributeLayout& attributelayout, const void* vertex_data, unsigned int data_size, unsigned int vertex_count) {
		if (!attributelayout.Does_BufferFits_Layout(vertex_data, data_size, vertex_count)) {
			return 0;
		}
		
		GFX_API::GFX_Mesh MESH;
		OGL4_MESH* GL_MESH = new OGL4_MESH;
		MESH.GL_ID = GL_MESH;
		glGenVertexArrays(1, &GL_MESH->VAO);
		glGenBuffers(1, &GL_MESH->VBO);
		glGenBuffers(1, &GL_MESH->EBO);
		glBindVertexArray(GL_MESH->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, GL_MESH->VBO);
		glBufferData(GL_ARRAY_BUFFER, data_size, vertex_data, GL_STATIC_DRAW);

		for (unsigned int attribute_index = 0; attribute_index < attributelayout.Attributes.size(); attribute_index++) {
			GFX_API::VertexAttribute* attribute = attributelayout.Attributes[attribute_index];
			//Reporting status
			{
				string Status = "Uploading Vertex Attribute: ";
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
		OGL4_MESH* MESHBUFFER = (OGL4_MESH*)Find_MeshBuffer_byBufferID(MeshBuffer_ID)->GL_ID;
		if (MESHBUFFER) {
			glDeleteVertexArrays(1, &MESHBUFFER->VAO);
			glDeleteBuffers(1, &MESHBUFFER->VBO);
			glDeleteBuffers(1, &MESHBUFFER->EBO);
			Find_MeshBuffer_byBufferID(MeshBuffer_ID)->GL_ID = nullptr;
			return;
		}

		TuranAPI::LOG_ERROR("Unload has failed because intended Mesh Buffer isn't found in OpenGL::GPU_ContentManager!");
	}

	void GPU_ContentManager::Unload_AllResources() {
		TuranAPI::LOG_NOTCODED("OpenGL::GPU_ContentManager->Unload_AllResources() isn't coded yet!", false);
	}

	void GPU_ContentManager::Upload_Texture(GFX_API::Texture_Resource* ASSET, unsigned int Asset_ID, bool Generate_Mipmap) {
		if (Find_GFXTexture_byID(Asset_ID)) {
			return;
		}
		unsigned int TEXTURE_DIMENSION = Find_Texture_Dimension(ASSET->Properties.DIMENSION);
		unsigned int TEXTURE_CHANNELs = Find_Texture_Channel_Type(ASSET->Properties.CHANNEL_TYPE);
		unsigned int TEXTURE_WRAPPING = Find_Texture_Wrapping(ASSET->Properties.WRAPPING);
		unsigned int TEXTURE_MIPMAPFILTER = Find_Texture_Mipmap_Filtering(ASSET->Properties.MIPMAP_FILTERING);
		unsigned int TEXTURE_VALUETYPE = Find_Texture_Value_Type(ASSET->Properties.VALUE_TYPE);


		unsigned int* TEXTUREID = new unsigned int(0);
		glGenTextures(1, TEXTUREID);
		glBindTexture(TEXTURE_DIMENSION, *TEXTUREID);


		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_WRAP_S, TEXTURE_WRAPPING);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_WRAP_T, TEXTURE_WRAPPING);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_MIN_FILTER, TEXTURE_MIPMAPFILTER);
		glTexParameteri(TEXTURE_DIMENSION, GL_TEXTURE_MAG_FILTER, TEXTURE_MIPMAPFILTER);

		glTexImage2D(TEXTURE_DIMENSION, 0, TEXTURE_CHANNELs, ASSET->WIDTH, ASSET->HEIGHT, 0, TEXTURE_CHANNELs, TEXTURE_VALUETYPE, ASSET->DATA);
		if (Generate_Mipmap) {
			glGenerateMipmap(TEXTURE_DIMENSION);
		}

		TuranAPI::LOG_NOTCODED("Texture_Resource should be able to store its own Mipmaps in Disk and we should upload them one by one here!\n", false);

		GFX_API::GFX_Texture TEXTURE;
		TEXTURE.GL_ID = TEXTUREID;
		TEXTURE.ASSET_ID = Asset_ID;
		TEXTUREs.push_back(TEXTURE);
	}
	void GPU_ContentManager::Unload_Texture(unsigned int TEXTURE_ID) {
		glDeleteTextures(1, (unsigned int*)Find_GFXTexture_byID(TEXTURE_ID)->GL_ID);
	}
	void GPU_ContentManager::Link_MaterialType(GFX_API::Material_Type* MATTYPE_ASSET, string* compilation_status) {
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
		glDeleteProgram(*(unsigned int*)Find_GFXShaderProgram_byID(Asset_ID)->GL_ID);
	}
	void GPU_ContentManager::Compile_ShaderSource(GFX_API::ShaderSource_Resource* SHADER, unsigned int Asset_ID, string* compilation_status) {
		unsigned int STAGE = Find_ShaderStage(SHADER->STAGE);

		unsigned int* SHADER_o = new unsigned int(0);
		*SHADER_o = glCreateShader(STAGE);
		auto x = SHADER->SOURCE_CODE.c_str();
		glShaderSource(*SHADER_o, 1, &x, NULL);
		glCompileShader(*SHADER_o);

		//Check compile issues!
		int success;
		char vert_infolog[512];
		glGetShaderiv(*SHADER_o, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(*SHADER_o, 512, NULL, vert_infolog);
			compilation_status->append(vert_infolog);
			TuranAPI::LOG_ERROR(compilation_status->c_str());
			return;
		}
		GFX_API::GFX_ShaderSource shadersource;
		shadersource.ASSET_ID = Asset_ID;
		shadersource.GL_ID = SHADER_o;
		SHADERSOURCEs.push_back(shadersource);
		compilation_status->append("Succesfully compiled!");
	}
	void GPU_ContentManager::Delete_ShaderSource(unsigned int Asset_ID) {
		glDeleteShader(*(unsigned int*)Find_GFXShaderSource_byID(Asset_ID)->GL_ID);
	}


	unsigned int GPU_ContentManager::Create_RenderTarget(unsigned int WIDTH, unsigned int HEIGTH, GFX_API::TEXTURE_DIMENSIONs DIMENSION,
		GFX_API::TEXTURE_TYPEs FORMAT, GFX_API::UNIFORMTYPE VALUETYPE, bool Usable_as_Texture) {
		GFX_API::RenderTarget RT;
		RT.WIDTH = WIDTH; RT.HEIGHT = HEIGTH; RT.DIMENSION = DIMENSION; RT.FORMAT = FORMAT;
		RT.FORMAT_VALUETYPE = VALUETYPE; RT.Usable_as_Texture = Usable_as_Texture;

		//Create a Texture!
		if (Usable_as_Texture) {
			unsigned int GL_FORMAT = Find_RenderTarget_Format_Type(FORMAT);
			unsigned int GL_DIMENSION = Find_Texture_Dimension(DIMENSION);
			unsigned int GL_VALUE_TYPE = Find_Texture_Value_Type(VALUETYPE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			RT.GL_ID = new unsigned int(0);
			glGenTextures(1, (unsigned int*)RT.GL_ID);
			glBindTexture(GL_TEXTURE_2D, *(unsigned int*)RT.GL_ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_FORMAT, WIDTH, HEIGTH, 0, GL_FORMAT, GL_VALUE_TYPE, NULL);
			{
				string status;
				status.append("Created a Render Target's OpenGL ID: ");
				status.append(std::to_string(*(unsigned int*)RT.GL_ID).c_str());
				TuranAPI::LOG_STATUS(status);
			}

			Check_GL_Errors("Created a Render Texture!");
		}
		//Create a Render Buffer
		else {
			TuranAPI::LOG_NOTCODED("Render Buffer creation isn't supported, you should set Usable_as_Texture to true!\n", true);
			return 0;
		}

		RT.ID = Create_RenderTargetID();
		RTs.push_back(RT);
		return RT.ID;
	}
	void GPU_ContentManager::Delete_RenderTarget(unsigned int RT_ID) {
		GFX_API::RenderTarget* RT = nullptr;
		for (unsigned int i = 0; i < RTs.size(); i++) {
			RT = &RTs[i];
			if (RT->ID == RT_ID) {
				if (RT->Usable_as_Texture) {
					glDeleteTextures(1, (unsigned int*)RT->GL_ID);
				}
				else {
					glDeleteRenderbuffers(1, (unsigned int*)RT->GL_ID);
				}
				RTs.erase(RTs.begin() + i);
			}
		}
	}

	unsigned int GPU_ContentManager::Create_Framebuffer() {
		GFX_API::Framebuffer FB;
		
		FB.GL_ID = new unsigned int(0);
		glGenFramebuffers(1, (unsigned int*)FB.GL_ID);
		Check_GL_Errors("After creating a Framebuffer!");
		FB.ID = Create_FrameBufferID();
		FBs.push_back(FB);
		return FB.ID;
	}

	void GPU_ContentManager::Attach_RenderTarget_toFramebuffer(const GFX_API::Framebuffer::RT_SLOT* RT_SLOT, GFX_API::RT_ATTACHMENTs ATTACHMENT_TYPE, unsigned int FB_ID) {
		GFX_API::Framebuffer* FB = nullptr;
		FB = Find_Framebuffer_byGFXID(FB_ID);

		GFX_API::RenderTarget* RT = nullptr;
		RT = Find_RenderTarget_byID(RT_SLOT->RT_ID);
		unsigned int RT_GLID = *(unsigned int*)RT->GL_ID;

		glBindFramebuffer(GL_FRAMEBUFFER, *(unsigned int*)Find_Framebuffer_byGFXID(FB_ID)->GL_ID);
		if (RT->Usable_as_Texture) {
			glBindTexture(GL_TEXTURE_2D, RT_GLID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, Find_RenderTarget_AttachmentType(ATTACHMENT_TYPE), GL_TEXTURE_2D, RT_GLID, 0);
		}
		else {
			TuranAPI::LOG_CRASHING("Creating a Render Buffer for RT isn't supported for now, so you can't bind it to Framebuffer! There is a problem somewhere");
			return;
		}
		Check_ActiveFramebuffer_Status(std::to_string(FB->ID).c_str());


		FB->BOUND_RTs.push_back(*RT_SLOT);
	}


	void GPU_ContentManager::Delete_Framebuffer(unsigned int Framebuffer_ID) {
		glDeleteFramebuffers(1, (unsigned int*)Find_Framebuffer_byGFXID(Framebuffer_ID)->GL_ID);
	}
}