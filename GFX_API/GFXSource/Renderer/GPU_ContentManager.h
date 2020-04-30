#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_ENUMs.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Material_Type_Resource.h"
#include "GFX_Resource.h"


namespace GFX_API {

	/* Vertex Attribute Layout Specification!
	If Stride = 0, attribute data is contiunous in memory.
	If Start_Offset is 0, that means previous attribute is contiunous and this attributes starts where previous attribute ends
	If you want previous attribute is interleaved, but this attribute is continous; you should set Start_Offset by yourself, Start_Offset = 0 will cause undefined behaviour.
	Buffer Layout have to be: Interleaved attributes first, Continuous attributes later. You can't have an interleaved attribute data after an continous attribute or mix like Interleaved - Continuous - Interleaved 

				EXAMPLES: Position(vec3) Attribute Data = 1, TextCoord(vec2) Data = 2, VertexColor(vec3) Data = 3, VertexNormal(vec3) Data = 4
	1) Memory: 1,1,1,2,2,2,3,3,3,4,4,4
	All attributes -> Stride = 0, Start_Offset = 0
	2) Memory: 1,2,1,2,1,2,3,3,3,4,4,4
	Position Attributes -> Stride = sizeof(vec3) + sizeof(vec2), Start_Offset = 0; TextCoord Attribute-> Stride = sizeof(vec3) + sizeof(vec2), Start_Offset = sizeof(vec3); VertexColor Attribute -> Stride = 0, Start_Offset = (sizeof(vec3) + sizeof(vec2)) * vertex_count;
		VertexNormal Attribute -> Stride = 0, Start_Offset = 0
	3) Memory: 1,1,1,2,3,4,2,3,4,2,3,4
	This is not allowed; always Interleaved attributes first, then Continuous!
	Note: In example 2, if you set VertexColor's Start_Offset to 0; that would cause undefined behaviour because previous attribute (TextCoord) isn't contiunous. 
	With vertex count, I could find where to start the continuous attribute even if the previous attribute is interleaved but this requires so many code and costs some performance
	because we need all the previous attributes and possibilities rises permutational (nested loops needed).
	Note: Both example 2 and 3 should be created for each mesh, because they need vertex count! 
	*/
	struct GFXAPI VertexAttribute {
		String AttributeName;
		UNIFORMTYPE DATATYPE;
		unsigned char Index;
		size_t Stride;
		size_t Start_Offset;
		VertexAttribute& operator=(const VertexAttribute& attribute);
	};
	
	struct GFXAPI VertexAttributeLayout {
		VertexAttributeLayout(unsigned char AttributeNumber);
		VertexAttributeLayout& operator=(const VertexAttributeLayout& layout);
		Vector<VertexAttribute*> Attributes;
		//All of the attributes are per vertex, so this is the size of attribute data per vertex
		unsigned int size_pervertex;
		void Calculate_SizeperVertex();
		bool VerifyAttributeLayout() const;
		bool Does_BufferFits_Layout(const void* vertex_data, size_t datasize_inbytes, size_t vertex_count) const;
		//Fail value is -1
		char Find_AttributeIndex_byName(const char* Attribute_Name) const;
		//Gathers all data of the specified attribute somewhere in the memory!
		//Return pointer is the pointer of gathered data and last argument data_size is the size of the data!
		//If you are not sure whether the vertex data fits your attribute layout, you should use Does_BufferFits_Layout() first!
		//If you don't know your attribute index, you can use Find
		void* Gather_AttributeData(const void* vertex_data, size_t datasize_inbytes, size_t vertex_count, unsigned char Attribute_Index, size_t& data_size) const;
	};

	/*RenderingComponents aren't supported here because every RenderGraph will define how it handles RenderComponents
	Nonetheless, the RenderingComponent's data should be in GPU so we should provide functions to send a buffer to GPU here

	All resources (except Meshes) are coupled with their GPU representations in responsible GFX_API's classes.
	That means, you can use your Asset_ID in GFX operations! If you didn't understand, you can go to OpenGL4.
	But because there is no concept of Mesh in GFX, you have to use Upload_MeshBuffer's returned ID in GFX operations.
	
	When Material Types are compiled, they're written to disk! So you should use Delete_MaterialType when you don't need a MaterialType anymore!
	I'd like to keep this process transparent and give the control to the user but the scope of the project is high enough to not allow this.
	I didn't want to support recompile, because this causes an unnecessary complex coupling between MaterialType-StoredBinaryData-FileSystem-GPUContentManager
	If user wants to recompile all the Material Types, he can delete all of them and compile again.
	*/
	class GFXAPI GPU_ContentManager {
	protected:
		Bitset MESHID_BITSET;
		unsigned int Create_MeshBufferID();
		void Delete_MeshBufferID(unsigned int ID);

		Bitset RTID_BITSET;
		unsigned int Create_RenderTargetID();
		void Delete_RenderTargetID(unsigned int ID);

		Bitset FBID_BITSET;
		unsigned int Create_FrameBufferID();
		void Delete_FrameBufferID(unsigned int ID);
	public:
		GPU_ContentManager();
		~GPU_ContentManager();

		virtual void Unload_AllResources() = 0;

		//Return MeshBufferID to use in Draw Calls
		virtual unsigned int Upload_MeshBuffer(const VertexAttributeLayout& attributelayout, const void* vertex_data, unsigned int data_size, unsigned int vertex_count) = 0;
		//When you call this function, Draw Calls that uses this ID may draw another Mesh or crash
		virtual void Unload_MeshBuffer(unsigned int MeshBuffer_ID) = 0;


		virtual void Upload_Texture(Texture_Resource* TEXTURE_ASSET, bool Generate_Mipmap) = 0;
		virtual void Unload_Texture(unsigned int ASSET_ID) = 0;

		virtual void Compile_ShaderSource(ShaderSource* SHADER, String* compilation_status) = 0;
		virtual void Delete_ShaderSource(unsigned int Shader_ID) = 0;
		virtual void Link_MaterialType(Material_Type_Resource* MATTYPE_ASSET, String* compilation_status) = 0;
		virtual void Delete_MaterialType(unsigned int Asset_ID) = 0;


		virtual unsigned int Create_RenderTarget(unsigned int WIDTH, unsigned int HEIGTH, TEXTURE_DIMENSIONs DIMENSION,
			TEXTURE_TYPEs FORMAT, UNIFORMTYPE FORMAT_VALUETYPE, bool Usable_as_Texture) = 0;
		virtual const GFX_API::RenderTarget& Find_RenderTargetProperties_byID(unsigned int RT_ID) = 0;
		virtual void Delete_RenderTarget(unsigned int RT_ID) = 0;

		virtual unsigned int Create_Framebuffer() = 0;
		virtual void Attach_RenderTarget_toFramebuffer(unsigned int RT_ID, GFX_API::RT_ATTACHMENTs ATTACHMENT_TYPE, unsigned int FB_ID) = 0;
		virtual void Delete_Framebuffer(unsigned int Framebuffer_ID) = 0;
	};
}