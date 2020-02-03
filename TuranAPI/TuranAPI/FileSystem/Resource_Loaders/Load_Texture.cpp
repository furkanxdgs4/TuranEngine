#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include "TuranAPI/FileSystem/Resource_Types/Texture_Resource.h"
#include "TuranAPI/FileSystem/DataFormats/GameResource_generated.h"
using namespace TuranAPI::File_System;

TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureChannels(GameContent::TEXTURE_CHANNELs channel_type);
TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureDimension(GameContent::TEXTURE_DIMENSION dimension);
TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureValueType(GameContent::TEXTURE_VALUETYPE value_type);
TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureMipmapFiltering(GameContent::TEXTURE_MIPMAP_FILTERING mipmap_filtering);
TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureWrapping(GameContent::TEXTURE_WRAPPING wrapping);

GameContent::TEXTURE_CHANNELs Convert_in_GameContent_TextureChannels(TuranAPI::TuranAPI_ENUMs channel_type);
GameContent::TEXTURE_DIMENSION Convert_in_GameContent_TextureDimension(TuranAPI::TuranAPI_ENUMs dimension);
GameContent::TEXTURE_VALUETYPE Convert_in_GameContent_TextureValueType(TuranAPI::TuranAPI_ENUMs value_type);
GameContent::TEXTURE_MIPMAP_FILTERING Convert_in_GameContent_TextureMipmapFiltering(TuranAPI::TuranAPI_ENUMs mipmap_filtering);
GameContent::TEXTURE_WRAPPING Convert_in_GameContent_TextureWrapping(TuranAPI::TuranAPI_ENUMs wrapping);
unsigned int Sizeof_Real_ValueType(TuranAPI::TuranAPI_ENUMs value_type);
unsigned int Number_of_Channels(TuranAPI::TuranAPI_ENUMs channel_type);

Resource_Type* FileSystem::Load_Texture(void* data, unsigned int id, const string& path) {
	auto RESOURCE_typeless = GameContent::GetResource(data);
	if (RESOURCE_typeless == nullptr) {
		cout << "Error: Loading failed! Texture Type isn't a valid resource!\n";
		return nullptr;
	}
	cout << "Loading Texture Type ID: " << id << endl;
	auto RESOURCE = RESOURCE_typeless->TYPE_as_Texture();

	if (!RESOURCE) {
		cout << "Type isn't Texture Type, type definition is wrong!\n";
		return nullptr;
	}
	if (id > LAST_ID) {
		LAST_ID = id;
	}


	Texture_Resource* TEXTURE = new Texture_Resource;

	TEXTURE->WIDTH = RESOURCE->WIDTH();
	TEXTURE->HEIGHT = RESOURCE->HEIGHT();
	TEXTURE->Properties = Texture_Properties(
		Convert_in_TuranAPIEnums_TextureDimension(RESOURCE->DIMENSION()),
		Convert_in_TuranAPIEnums_TextureMipmapFiltering(RESOURCE->MIPMAP_FILTERING()),
		Convert_in_TuranAPIEnums_TextureWrapping(RESOURCE->WRAPPING()),
		Convert_in_TuranAPIEnums_TextureChannels(RESOURCE->CHANNELs()),
		Convert_in_TuranAPIEnums_TextureValueType(RESOURCE->VALUE_TYPE())
	);
	TEXTURE->DATA_SIZE = TEXTURE->WIDTH * TEXTURE->HEIGHT * Number_of_Channels(TEXTURE->Properties.CHANNEL_TYPE);
	TEXTURE->DATA = new unsigned char[TEXTURE->DATA_SIZE];
	
	//Fill the texture data to Texture_Resource from Flatbuffer!
	for (unsigned int i = 0; i < TEXTURE->DATA_SIZE; i++) {
		TEXTURE->DATA[i] = RESOURCE->DATA()->Get(i);
	}


	TEXTURE->ID = id;
	TEXTURE->PATH = path;
	string NAME = path.substr(path.find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	TEXTURE->NAME = NAME;
	return TEXTURE;
}

void Save_a_Texture_toDisk(Resource_Type* Texture_Data) {
	Texture_Resource* TEXTURE = (Texture_Resource*)Texture_Data;
	flatbuffers::FlatBufferBuilder builder(1024);


	//Fill the textrue data to Flatbuffer from Texture_Resource!
	unsigned char* texture_data_ptr = TEXTURE->DATA;
	vector<unsigned char> Texture_NaiveVector_DATA;
	for (unsigned int i = 0; i < TEXTURE->DATA_SIZE; i++) {
		Texture_NaiveVector_DATA.push_back(texture_data_ptr[i]);
	}
	auto TEXTURE_NaiveFlatbuffer_DATA = builder.CreateVector(Texture_NaiveVector_DATA);

	//Set texture properties and data to Flatbuffer!
	auto FINISHED_TEXTURE_DATA = GameContent::CreateTEXTURE(builder, TEXTURE->WIDTH, TEXTURE->HEIGHT, TEXTURE_NaiveFlatbuffer_DATA,
		Convert_in_GameContent_TextureDimension(TEXTURE->Properties.DIMENSION),
		Convert_in_GameContent_TextureChannels(TEXTURE->Properties.CHANNEL_TYPE),
		Convert_in_GameContent_TextureValueType(TEXTURE->Properties.VALUE_TYPE),
		Convert_in_GameContent_TextureMipmapFiltering(TEXTURE->Properties.MIPMAP_FILTERING),
		Convert_in_GameContent_TextureWrapping(TEXTURE->Properties.WRAPPING));
	auto FINISHED_RESOURCE = GameContent::CreateResource(builder, GameContent::Resource_Type_Texture, FINISHED_TEXTURE_DATA.Union());
	builder.Finish(FINISHED_RESOURCE);

	unsigned int data_size = builder.GetSize();

	cout << "Compiled Texture Resource to Flatbuffer type!\n";
	void* data_ptr = builder.GetBufferPointer();
	cout << "Exporting resource as a .texturecont: " << TEXTURE->PATH << endl;
	FileSystem::Overwrite_BinaryFile(TEXTURE->PATH, data_ptr, data_size);
}

unsigned int Sizeof_Real_ValueType(TuranAPI::TuranAPI_ENUMs value_type) {
	switch (value_type)
	{
	case TuranAPI::TuranAPI_ENUMs::VAR_UBYTE8:
		return 8;
	case TuranAPI::TuranAPI_ENUMs::VAR_BYTE8:
		return 8;
	case TuranAPI::TuranAPI_ENUMs::VAR_UINT32:
		return 32;
	case TuranAPI::TuranAPI_ENUMs::VAR_INT32:
		return 32;
	case TuranAPI::TuranAPI_ENUMs::VAR_FLOAT32:
		return 32;
	default:
		cout << "Error: Intended size of Real Valuetype isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
		return 0;
	}
}

unsigned int Number_of_Channels(TuranAPI::TuranAPI_ENUMs channel_type) {
	switch (channel_type) {
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGB:
		return 3;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGBA:
		return 4;
	default:
		cout << "Error: Intended Number_of_Channels() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
		return 0;
	}
}

GameContent::TEXTURE_CHANNELs Convert_in_GameContent_TextureChannels(TuranAPI::TuranAPI_ENUMs channel_type) {
	switch (channel_type)
	{
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGB:
		return GameContent::TEXTURE_CHANNELs_TEXTURE_RGB;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGBA:
		return GameContent::TEXTURE_CHANNELs_TEXTURE_RGBA;
	default:
		cout << "Error: Intended Convert_in_GameContent_TextureChannels() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

GameContent::TEXTURE_DIMENSION Convert_in_GameContent_TextureDimension(TuranAPI::TuranAPI_ENUMs dimension) {
	switch (dimension)
	{
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_2D:
		return GameContent::TEXTURE_DIMENSION_TEXTURE_2D;
	default:
		cout << "Error: Intended Convert_in_GameContent_TextureDimension() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

GameContent::TEXTURE_VALUETYPE Convert_in_GameContent_TextureValueType(TuranAPI::TuranAPI_ENUMs value_type) {
	switch (value_type) {
	case TuranAPI::TuranAPI_ENUMs::VAR_UBYTE8:
		return GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UBYTE8;
	case TuranAPI::TuranAPI_ENUMs::VAR_BYTE8:
		return GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_BYTE8;
	case TuranAPI::TuranAPI_ENUMs::VAR_UINT32:
		return GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UINT32;
	case TuranAPI::TuranAPI_ENUMs::VAR_INT32:
		return GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_INT32;
	case TuranAPI::TuranAPI_ENUMs::VAR_FLOAT32:
		return GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_FLOAT32;
	default:
		cout << "Error: Intended Convert_in_GameContent_TextureValueType() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

GameContent::TEXTURE_MIPMAP_FILTERING Convert_in_GameContent_TextureMipmapFiltering(TuranAPI::TuranAPI_ENUMs mipmap_filtering) {
	switch (mipmap_filtering) {
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_1MIP:
		return GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_1MIP;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_2MIP:
		return GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_2MIP;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_NEAREST_FROM_1MIP:
		return GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_1MIP;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_NEAREST_FROM_2MIP:
		return GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_2MIP;
	default:
		cout << "Error: Intended Convert_in_GameContent_TextureMipmapFiltering() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}
GameContent::TEXTURE_WRAPPING Convert_in_GameContent_TextureWrapping(TuranAPI::TuranAPI_ENUMs wrapping) {
	switch (wrapping) {
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_REPEAT:
		return GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_REPEAT;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_MIRRORED_REPEAT:
		return GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_MIRRORED_REPEAT;
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_CLAMP_TO_EDGE:
		return GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_CLAMP_TO_EDGE;
	default:
		cout << "Error: Intended Convert_in_GameContent_TextureWrapping() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}





TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureChannels(GameContent::TEXTURE_CHANNELs channel_type) {
	switch (channel_type)
	{
	case GameContent::TEXTURE_CHANNELs_TEXTURE_RGB:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGB;
	case GameContent::TEXTURE_CHANNELs_TEXTURE_RGBA:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGBA;
	default:
		cout << "Error: Intended Convert_in_TuranAPIEnums_TextureChannels() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureDimension(GameContent::TEXTURE_DIMENSION dimension) {
	switch (dimension)
	{
	case GameContent::TEXTURE_DIMENSION_TEXTURE_2D:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_2D;
	default:
		cout << "Error: Intended Convert_in_TuranAPIEnums_TextureDimension() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureValueType(GameContent::TEXTURE_VALUETYPE value_type) {
	switch (value_type) {
	case GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UBYTE8:
		return TuranAPI::TuranAPI_ENUMs::VAR_UBYTE8;
	case GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_BYTE8:
		return TuranAPI::TuranAPI_ENUMs::VAR_BYTE8;
	case GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_UINT32:
		return TuranAPI::TuranAPI_ENUMs::VAR_UINT32;
	case GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_INT32:
		return TuranAPI::TuranAPI_ENUMs::VAR_INT32;
	case GameContent::TEXTURE_VALUETYPE::TEXTURE_VALUETYPE_FLOAT32:
		return TuranAPI::TuranAPI_ENUMs::VAR_FLOAT32;
	default:
		cout << "Error: Intended Convert_in_TuranAPIEnums_TextureValueType() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureMipmapFiltering(GameContent::TEXTURE_MIPMAP_FILTERING mipmap_filtering) {
	switch (mipmap_filtering) {
	case GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_1MIP:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_1MIP;
	case GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_LINEAR_FROM_2MIP:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_2MIP;
	case GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_1MIP:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_NEAREST_FROM_1MIP;
	case GameContent::TEXTURE_MIPMAP_FILTERING::TEXTURE_MIPMAP_FILTERING_NEAREST_FROM_2MIP:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_NEAREST_FROM_2MIP;
	default:
		cout << "Error: Intended Convert_in_TuranAPIEnums_TextureMipmapFiltering() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}

TuranAPI::TuranAPI_ENUMs Convert_in_TuranAPIEnums_TextureWrapping(GameContent::TEXTURE_WRAPPING wrapping) {
	switch (wrapping) {
	case GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_REPEAT:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_REPEAT;
	case GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_MIRRORED_REPEAT:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_MIRRORED_REPEAT;
	case GameContent::TEXTURE_WRAPPING::TEXTURE_WRAPPING_CLAMP_TO_EDGE:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_CLAMP_TO_EDGE;
	default:
		cout << "Error: Intended Convert_in_TuranAPIEnums_TextureWrapping() isn't supported for now!\n";
		this_thread::sleep_for(chrono::seconds(5));
	}
}