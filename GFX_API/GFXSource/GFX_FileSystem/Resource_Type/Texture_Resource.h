#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_ENUMs.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

enum class RESOURCETYPEs : char;

namespace GFX_API {
	struct GFXAPI Texture_Properties {
		TEXTURE_DIMENSIONs DIMENSION = TEXTURE_DIMENSIONs::TEXTURE_2D;
		TEXTURE_MIPMAPFILTER MIPMAP_FILTERING = TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_1MIP;
		TEXTURE_WRAPPING WRAPPING = TEXTURE_WRAPPING::API_TEXTURE_REPEAT;
		TEXTURE_CHANNELs CHANNEL_TYPE = TEXTURE_CHANNELs::API_TEXTURE_RGB;
		UNIFORMTYPE VALUE_TYPE = UNIFORMTYPE::VAR_UBYTE8;
		Texture_Properties();
		Texture_Properties(TEXTURE_DIMENSIONs dimension, TEXTURE_MIPMAPFILTER mipmap_filtering = TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_1MIP,
			TEXTURE_WRAPPING wrapping = TEXTURE_WRAPPING::API_TEXTURE_REPEAT, TEXTURE_CHANNELs channel_type = TEXTURE_CHANNELs::API_TEXTURE_RGB, UNIFORMTYPE value_type = UNIFORMTYPE::VAR_UBYTE8);
	};

	class GFXAPI Texture_Resource : public TuranAPI::Resource_Type {
	public:
		Texture_Properties Properties;
		unsigned int WIDTH, HEIGHT, DATA_SIZE;
		unsigned char* DATA;
		Texture_Resource();

		virtual bool Verify_Resource_Data();
		virtual void Write_ToDisk(bool Verify_Data);
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator);
	};
}

