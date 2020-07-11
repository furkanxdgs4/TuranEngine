#include "EditorSource/Editor_Includes.h"
#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "GFXSource/GFX_FileSystem/DataFormats/GFXAPI_Resources_generated.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

using namespace GFX_API;
namespace TuranEditor {
	//HELPER FUNCTIONs

	TEXTURE_CHANNELs Convert_toTuranAPIEnums_TextureChannels(GFXAsset::TEXTURE_CHANNELs channel_type);
	TEXTURE_DIMENSIONs Convert_toTuranAPIEnums_TextureDimension(GFXAsset::TEXTURE_DIMENSION dimension);
	UNIFORMTYPE Convert_toTuranAPIEnums_TextureValueType(GFXAsset::TEXTURE_VALUETYPE value_type);
	TEXTURE_MIPMAPFILTER Convert_toTuranAPIEnums_TextureMipmapFiltering(GFXAsset::TEXTURE_MIPMAP_FILTERING mipmap_filtering);
	TEXTURE_WRAPPING Convert_toTuranAPIEnums_TextureWrapping(GFXAsset::TEXTURE_WRAPPING wrapping);

	GFXAsset::TEXTURE_CHANNELs Convert_toGameContent_TextureChannels(TEXTURE_CHANNELs channel_type);
	GFXAsset::TEXTURE_DIMENSION Convert_toGameContent_TextureDimension(TEXTURE_DIMENSIONs dimension);
	GFXAsset::TEXTURE_VALUETYPE Convert_toGameContent_TextureValueType(UNIFORMTYPE value_type);
	GFXAsset::TEXTURE_MIPMAP_FILTERING Convert_toGameContent_TextureMipmapFiltering(TEXTURE_MIPMAPFILTER mipmap_filtering);
	GFXAsset::TEXTURE_WRAPPING Convert_toGameContent_TextureWrapping(TEXTURE_WRAPPING wrapping);
	unsigned int Number_of_Channels(TEXTURE_CHANNELs channel_type);


	bool Verify_Texture(Resource_Identifier* IDENTIFIER) {
		Texture_Resource* TEXTURE = (Texture_Resource*)IDENTIFIER->DATA;
		if (!TEXTURE) {
			EDITOR_FILESYSTEM->Load_Resource(IDENTIFIER->ID);
		}
		if (TEXTURE->WIDTH > 0 && TEXTURE->HEIGHT > 0) {
			if (TEXTURE->DATA_SIZE > 0 && TEXTURE->DATA != nullptr) {
				if (TEXTURE->Properties.DIMENSION != TEXTURE_DIMENSIONs::TEXTURE_2D) {
					TuranAPI::Breakpoint("Texture Resource isn't verified because it's dimension is something unsupported!\nIt has a different Enum value than API_TEXTURE_2D");
					return false;
				}
				else { return true; }
			}
			else {
				TuranAPI::Breakpoint("Texture Resource isn't verified because it has invalid Data Size or Data is nullptr!");
				return false;
			}
		}
		else {
			TuranAPI::Breakpoint("Texture Resource isn't verified because it has invalid Width or Height!");
			return false;
		}
	}


	void* Load_Texture(Resource_Identifier* IDENTIFIER) {
		unsigned int data_size = 0;
		void* file_data = TAPIFILESYSTEM::Read_BinaryFile(IDENTIFIER->PATH.c_str(), data_size, LASTUSEDALLOCATOR);
		if (!file_data) {
			TuranAPI::LOG_ERROR("Loading failed! Texture file isn't found!");
			return nullptr;
		}
		auto RESOURCE_typeless = GFXAsset::GetResource(file_data);
		if (RESOURCE_typeless == nullptr) {
			TuranAPI::LOG_ERROR("Loading failed! Texture Type isn't a valid resource!");
			return nullptr;
		}
		std::cout << "Loading Texture Type ID: " << IDENTIFIER->ID << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Texture();

		if (!RESOURCE) {
			std::cout << "Type isn't Texture Type, type definition is wrong!\n";
			return;
		}

		Texture_Resource* TEXTURE = new Texture_Resource;
		IDENTIFIER->DATA = TEXTURE;
		TEXTURE->WIDTH = RESOURCE->WIDTH();
		TEXTURE->HEIGHT = RESOURCE->HEIGHT();
		TEXTURE->Properties = Texture_Properties(
			Convert_toTuranAPIEnums_TextureDimension(RESOURCE->DIMENSION()),
			Convert_toTuranAPIEnums_TextureMipmapFiltering(RESOURCE->MIPMAP_FILTERING()),
			Convert_toTuranAPIEnums_TextureWrapping(RESOURCE->WRAPPING()),
			Convert_toTuranAPIEnums_TextureChannels(RESOURCE->CHANNELs()),
			Convert_toTuranAPIEnums_TextureValueType(RESOURCE->VALUE_TYPE())
		);
		TEXTURE->DATA_SIZE = TEXTURE->WIDTH * TEXTURE->HEIGHT * Number_of_Channels(TEXTURE->Properties.CHANNEL_TYPE);
		TEXTURE->DATA = new unsigned char[TEXTURE->DATA_SIZE];

		//Fill the texture data to Texture_Resource from Flatbuffer!
		for (unsigned int i = 0; i < TEXTURE->DATA_SIZE; i++) {
			TEXTURE->DATA[i] = RESOURCE->DATA()->Get(i);
		}
	}


	void Save_Texture(Resource_Identifier* IDENTIFIER) {
		Texture_Resource* TEXTURE = (Texture_Resource*)IDENTIFIER->DATA;

		flatbuffers::FlatBufferBuilder builder(1024);

		//Fill the textrue data to Flatbuffer from Texture_Resource!
		std::vector<unsigned char> Texture_Naivevector_DATA;
		Texture_Naivevector_DATA.resize(TEXTURE->DATA_SIZE);
		memcpy(Texture_Naivevector_DATA.data(), TEXTURE->DATA, TEXTURE->DATA_SIZE);
		auto TEXTURE_NaiveFlatbuffer_DATA = builder.CreateVector(Texture_Naivevector_DATA);

		//Set texture properties and data to Flatbuffer!
		auto FINISHED_TEXTURE_DATA = GFXAsset::CreateTEXTURE(builder, TEXTURE->WIDTH, TEXTURE->HEIGHT, TEXTURE_NaiveFlatbuffer_DATA,
			Convert_toGameContent_TextureDimension(TEXTURE->Properties.DIMENSION),
			Convert_toGameContent_TextureChannels(TEXTURE->Properties.CHANNEL_TYPE),
			Convert_toGameContent_TextureValueType(TEXTURE->Properties.VALUE_TYPE),
			Convert_toGameContent_TextureMipmapFiltering(TEXTURE->Properties.MIPMAP_FILTERING),
			Convert_toGameContent_TextureWrapping(TEXTURE->Properties.WRAPPING));
		auto FINISHED_RESOURCE = GFXAsset::CreateResource(builder, GFXAsset::Resource_Type_Texture, FINISHED_TEXTURE_DATA.Union());
		builder.Finish(FINISHED_RESOURCE);

		unsigned int data_size = builder.GetSize();

		std::cout << "Compiled Texture Resource to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();
		std::cout << "Exporting resource as a .texturecont: " << IDENTIFIER->PATH << std::endl;
		TAPIFILESYSTEM::Overwrite_BinaryFile(IDENTIFIER->PATH.c_str(), data_ptr, data_size);
		Texture_Naivevector_DATA.clear();
	}



	//HELPER FUNCTIONs

	unsigned int Number_of_Channels(TEXTURE_CHANNELs channel_type) {
		switch (channel_type) {
		case TEXTURE_CHANNELs::API_TEXTURE_RGB:
			return 3;
		case TEXTURE_CHANNELs::API_TEXTURE_RGBA:
			return 4;
		default:
			std::cout << "Error: Intended Number_of_Channels() isn't supported for now!\n";
			SLEEP_THREAD(5);
			return 0;
		}
	}
	GFXAsset::TEXTURE_CHANNELs Convert_toGameContent_TextureChannels(TEXTURE_CHANNELs channel_type) {
		switch (channel_type)
		{
		case TEXTURE_CHANNELs::API_TEXTURE_RGB:
			return GFXAsset::TEXTURE_CHANNELs_TEXTURE_RGB;
		case TEXTURE_CHANNELs::API_TEXTURE_RGBA:
			return GFXAsset::TEXTURE_CHANNELs_TEXTURE_RGBA;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureChannels() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	GFXAsset::TEXTURE_DIMENSION Convert_toGameContent_TextureDimension(TEXTURE_DIMENSIONs dimension) {
		switch (dimension)
		{
		case TEXTURE_DIMENSIONs::TEXTURE_2D:
			return GFXAsset::TEXTURE_DIMENSION_TEXTURE_2D;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureDimension() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	GFXAsset::TEXTURE_VALUETYPE Convert_toGameContent_TextureValueType(UNIFORMTYPE value_type) {
		switch (value_type) {
		case UNIFORMTYPE::VAR_UBYTE8:
			return GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UBYTE8;
		case UNIFORMTYPE::VAR_BYTE8:
			return GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_BYTE8;
		case UNIFORMTYPE::VAR_UINT32:
			return GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UINT32;
		case UNIFORMTYPE::VAR_INT32:
			return GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_INT32;
		case UNIFORMTYPE::VAR_FLOAT32:
			return GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_FLOAT32;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureValueType() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	GFXAsset::TEXTURE_MIPMAP_FILTERING Convert_toGameContent_TextureMipmapFiltering(TEXTURE_MIPMAPFILTER mipmap_filtering) {
		switch (mipmap_filtering) {
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_1MIP:
			return GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_1MIP;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_2MIP:
			return GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_2MIP;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_1MIP:
			return GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_1MIP;
		case TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_2MIP:
			return GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_2MIP;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureMipmapFiltering() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	GFXAsset::TEXTURE_WRAPPING Convert_toGameContent_TextureWrapping(TEXTURE_WRAPPING wrapping) {
		switch (wrapping) {
		case TEXTURE_WRAPPING::API_TEXTURE_REPEAT:
			return GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_REPEAT;
		case TEXTURE_WRAPPING::API_TEXTURE_MIRRORED_REPEAT:
			return GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_MIRRORED_REPEAT;
		case TEXTURE_WRAPPING::API_TEXTURE_CLAMP_TO_EDGE:
			return GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_CLAMP_TO_EDGE;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureWrapping() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}





	TEXTURE_CHANNELs Convert_toTuranAPIEnums_TextureChannels(GFXAsset::TEXTURE_CHANNELs channel_type) {
		switch (channel_type)
		{
		case GFXAsset::TEXTURE_CHANNELs_TEXTURE_RGB:
			return TEXTURE_CHANNELs::API_TEXTURE_RGB;
		case GFXAsset::TEXTURE_CHANNELs_TEXTURE_RGBA:
			return TEXTURE_CHANNELs::API_TEXTURE_RGBA;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureChannels() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	TEXTURE_DIMENSIONs Convert_toTuranAPIEnums_TextureDimension(GFXAsset::TEXTURE_DIMENSION dimension) {
		switch (dimension)
		{
		case GFXAsset::TEXTURE_DIMENSION_TEXTURE_2D:
			return TEXTURE_DIMENSIONs::TEXTURE_2D;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureDimension() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	UNIFORMTYPE Convert_toTuranAPIEnums_TextureValueType(GFXAsset::TEXTURE_VALUETYPE value_type) {
		switch (value_type) {
		case GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UBYTE8:
			return UNIFORMTYPE::VAR_UBYTE8;
		case GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_BYTE8:
			return UNIFORMTYPE::VAR_BYTE8;
		case GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UINT32:
			return UNIFORMTYPE::VAR_UINT32;
		case GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_INT32:
			return UNIFORMTYPE::VAR_INT32;
		case GFXAsset::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_FLOAT32:
			return UNIFORMTYPE::VAR_FLOAT32;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureValueType() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	TEXTURE_MIPMAPFILTER Convert_toTuranAPIEnums_TextureMipmapFiltering(GFXAsset::TEXTURE_MIPMAP_FILTERING mipmap_filtering) {
		switch (mipmap_filtering) {
		case GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_1MIP:
			return TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_1MIP;
		case GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_2MIP:
			return TEXTURE_MIPMAPFILTER::API_TEXTURE_LINEAR_FROM_2MIP;
		case GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_1MIP:
			return TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_1MIP;
		case GFXAsset::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_2MIP:
			return TEXTURE_MIPMAPFILTER::API_TEXTURE_NEAREST_FROM_2MIP;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureMipmapFiltering() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	TEXTURE_WRAPPING Convert_toTuranAPIEnums_TextureWrapping(GFXAsset::TEXTURE_WRAPPING wrapping) {
		switch (wrapping) {
		case GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_REPEAT:
			return TEXTURE_WRAPPING::API_TEXTURE_REPEAT;
		case GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_MIRRORED_REPEAT:
			return TEXTURE_WRAPPING::API_TEXTURE_MIRRORED_REPEAT;
		case GFXAsset::TEXTURE_WRAPPING::TEXTURE_WRAPPING_CLAMP_TO_EDGE:
			return TEXTURE_WRAPPING::API_TEXTURE_CLAMP_TO_EDGE;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureWrapping() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
}