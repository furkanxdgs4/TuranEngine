#include "Texture_Resource.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include <flatbuffers/flatbuffers.h>
#include "GFXSource/GFX_FileSystem/DataFormats/GFXAPI_Resources_generated.h"
#include "TuranAPI/FileSystem/Resource_Types/ResourceTYPEs.h"

namespace GFX_API {
	Texture_Properties::Texture_Properties() {}
	Texture_Properties::Texture_Properties(TEXTURE_DIMENSIONs dimension, TEXTURE_MIPMAPFILTER mipmap_filtering, TEXTURE_WRAPPING wrapping, TEXTURE_CHANNELs channel_type, UNIFORMTYPE value_type)
		: DIMENSION(dimension), MIPMAP_FILTERING(mipmap_filtering), WRAPPING(wrapping), CHANNEL_TYPE(channel_type), VALUE_TYPE(value_type) {}

	Texture_Resource::Texture_Resource() : Resource_Type(RESOURCETYPEs::GFXAPI_TEXTURE) {

	}


	bool Texture_Resource::Verify_Resource_Data() {
		if (NAME.length() == 0) {
			TuranAPI::Breakpoint("NAME is empty!");
			return false;
		}
		else if (PATH.length() == 0) {
			TuranAPI::Breakpoint("PATH is empty!");
			return false;
		}

		if (WIDTH > 0 && HEIGHT > 0) {
			if (DATA_SIZE > 0 && DATA != nullptr) {
				if (Properties.DIMENSION != TEXTURE_DIMENSIONs::TEXTURE_2D) {
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

		//I don't know why would the program reach here!
		return false;
	}

	TEXTURE_CHANNELs Convert_in_TuranAPIEnums_TextureChannels(GFXAsset::TEXTURE_CHANNELs channel_type);
	TEXTURE_DIMENSIONs Convert_in_TuranAPIEnums_TextureDimension(GFXAsset::TEXTURE_DIMENSION dimension);
	UNIFORMTYPE Convert_in_TuranAPIEnums_TextureValueType(GFXAsset::TEXTURE_VALUETYPE value_type);
	TEXTURE_MIPMAPFILTER Convert_in_TuranAPIEnums_TextureMipmapFiltering(GFXAsset::TEXTURE_MIPMAP_FILTERING mipmap_filtering);
	TEXTURE_WRAPPING Convert_in_TuranAPIEnums_TextureWrapping(GFXAsset::TEXTURE_WRAPPING wrapping);

	GFXAsset::TEXTURE_CHANNELs Convert_in_GameContent_TextureChannels(TEXTURE_CHANNELs channel_type);
	GFXAsset::TEXTURE_DIMENSION Convert_in_GameContent_TextureDimension(TEXTURE_DIMENSIONs dimension);
	GFXAsset::TEXTURE_VALUETYPE Convert_in_GameContent_TextureValueType(UNIFORMTYPE value_type);
	GFXAsset::TEXTURE_MIPMAP_FILTERING Convert_in_GameContent_TextureMipmapFiltering(TEXTURE_MIPMAPFILTER mipmap_filtering);
	GFXAsset::TEXTURE_WRAPPING Convert_in_GameContent_TextureWrapping(TEXTURE_WRAPPING wrapping);
	unsigned int Number_of_Channels(TEXTURE_CHANNELs channel_type);

	void Texture_Resource::Load_FromDisk(void* data, unsigned int id, const char* path, IAllocator* Allocator) {
		auto RESOURCE_typeless = GFXAsset::GetResource(data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Texture Type isn't a valid resource!\n";
			return;
		}
		std::cout << "Loading Texture Type ID: " << id << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Texture();

		if (!RESOURCE) {
			std::cout << "Type isn't Texture Type, type definition is wrong!\n";
			return;
		}

		WIDTH = RESOURCE->WIDTH();
		HEIGHT = RESOURCE->HEIGHT();
		Properties = Texture_Properties(
			Convert_in_TuranAPIEnums_TextureDimension(RESOURCE->DIMENSION()),
			Convert_in_TuranAPIEnums_TextureMipmapFiltering(RESOURCE->MIPMAP_FILTERING()),
			Convert_in_TuranAPIEnums_TextureWrapping(RESOURCE->WRAPPING()),
			Convert_in_TuranAPIEnums_TextureChannels(RESOURCE->CHANNELs()),
			Convert_in_TuranAPIEnums_TextureValueType(RESOURCE->VALUE_TYPE())
		);
		DATA_SIZE = WIDTH * HEIGHT * Number_of_Channels(Properties.CHANNEL_TYPE);
		DATA = new unsigned char[DATA_SIZE];

		//Fill the texture data to Texture_Resource from Flatbuffer!
		for (unsigned int i = 0; i < DATA_SIZE; i++) {
			DATA[i] = RESOURCE->DATA()->Get(i);
		}

		
		ID = id;
		PATH = path;
		std::string name = path;
		name = name.substr(name.find_last_of('/') + 1);
		name = name.substr(0, name.find_last_of('.'));
		NAME = name.c_str();
	}

	void Texture_Resource::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		flatbuffers::FlatBufferBuilder builder(1024);


		//Fill the textrue data to Flatbuffer from Texture_Resource!
		std::vector<unsigned char> Texture_NaiveVector_DATA;
		Texture_NaiveVector_DATA.resize(DATA_SIZE);
		memcpy(Texture_NaiveVector_DATA.data(), DATA, DATA_SIZE);
		auto TEXTURE_NaiveFlatbuffer_DATA = builder.CreateVector(Texture_NaiveVector_DATA);

		//Set texture properties and data to Flatbuffer!
		auto FINISHED_TEXTURE_DATA = GFXAsset::CreateTEXTURE(builder, WIDTH, HEIGHT, TEXTURE_NaiveFlatbuffer_DATA,
			Convert_in_GameContent_TextureDimension(Properties.DIMENSION),
			Convert_in_GameContent_TextureChannels(Properties.CHANNEL_TYPE),
			Convert_in_GameContent_TextureValueType(Properties.VALUE_TYPE),
			Convert_in_GameContent_TextureMipmapFiltering(Properties.MIPMAP_FILTERING),
			Convert_in_GameContent_TextureWrapping(Properties.WRAPPING));
		auto FINISHED_RESOURCE = GFXAsset::CreateResource(builder, GFXAsset::Resource_Type_Texture, FINISHED_TEXTURE_DATA.Union());
		builder.Finish(FINISHED_RESOURCE);

		unsigned int data_size = builder.GetSize();

		std::cout << "Compiled Texture Resource to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();
		std::cout << "Exporting resource as a .texturecont: " << PATH << std::endl;
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH, data_ptr, data_size);
		Texture_NaiveVector_DATA.clear();
	}
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
	GFXAsset::TEXTURE_CHANNELs Convert_in_GameContent_TextureChannels(TEXTURE_CHANNELs channel_type) {
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
	GFXAsset::TEXTURE_DIMENSION Convert_in_GameContent_TextureDimension(TEXTURE_DIMENSIONs dimension) {
		switch (dimension)
		{
		case TEXTURE_DIMENSIONs::TEXTURE_2D:
			return GFXAsset::TEXTURE_DIMENSION_TEXTURE_2D;
		default:
			std::cout << "Error: Intended Convert_in_GameContent_TextureDimension() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	GFXAsset::TEXTURE_VALUETYPE Convert_in_GameContent_TextureValueType(UNIFORMTYPE value_type) {
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
	GFXAsset::TEXTURE_MIPMAP_FILTERING Convert_in_GameContent_TextureMipmapFiltering(TEXTURE_MIPMAPFILTER mipmap_filtering) {
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
	GFXAsset::TEXTURE_WRAPPING Convert_in_GameContent_TextureWrapping(TEXTURE_WRAPPING wrapping) {
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





	TEXTURE_CHANNELs Convert_in_TuranAPIEnums_TextureChannels(GFXAsset::TEXTURE_CHANNELs channel_type) {
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
	TEXTURE_DIMENSIONs Convert_in_TuranAPIEnums_TextureDimension(GFXAsset::TEXTURE_DIMENSION dimension) {
		switch (dimension)
		{
		case GFXAsset::TEXTURE_DIMENSION_TEXTURE_2D:
			return TEXTURE_DIMENSIONs::TEXTURE_2D;
		default:
			std::cout << "Error: Intended Convert_in_TuranAPIEnums_TextureDimension() isn't supported for now!\n";
			SLEEP_THREAD(5);
		}
	}
	UNIFORMTYPE Convert_in_TuranAPIEnums_TextureValueType(GFXAsset::TEXTURE_VALUETYPE value_type) {
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
	TEXTURE_MIPMAPFILTER Convert_in_TuranAPIEnums_TextureMipmapFiltering(GFXAsset::TEXTURE_MIPMAP_FILTERING mipmap_filtering) {
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
	TEXTURE_WRAPPING Convert_in_TuranAPIEnums_TextureWrapping(GFXAsset::TEXTURE_WRAPPING wrapping) {
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