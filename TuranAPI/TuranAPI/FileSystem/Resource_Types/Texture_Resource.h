#pragma once
#include "TuranAPI/API_includes.h"
#include "Resource_Type.h"

namespace TuranAPI {
	namespace File_System {
		struct TURANAPI Texture_Properties {
			TuranAPI_ENUMs DIMENSION = TuranAPI_ENUMs::API_TEXTURE_2D, MIPMAP_FILTERING = TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_1MIP, WRAPPING = TuranAPI_ENUMs::API_TEXTURE_REPEAT,
				CHANNEL_TYPE = TuranAPI_ENUMs::API_TEXTURE_RGB, VALUE_TYPE = TuranAPI_ENUMs::VAR_UBYTE8;
			Texture_Properties();
			Texture_Properties(TuranAPI_ENUMs dimension, TuranAPI_ENUMs mipmap_filtering = TuranAPI_ENUMs::API_TEXTURE_LINEAR_FROM_1MIP
				, TuranAPI_ENUMs wrapping = TuranAPI_ENUMs::API_TEXTURE_REPEAT, TuranAPI_ENUMs channel_type = TuranAPI_ENUMs::API_TEXTURE_RGB, TuranAPI_ENUMs value_type = TuranAPI_ENUMs::VAR_UBYTE8);
		};

		class TURANAPI Texture_Resource : public Resource_Type {
		public:
			static vector<Texture_Resource*> ALL_TEXTUREs;

			Texture_Properties Properties;
			int WIDTH, HEIGHT;
			unsigned char* DATA;
			unsigned int DATA_SIZE;
			//GL_ID is the ID of the texture in the GPU
			unsigned int GL_ID = 0;

			virtual TuranAPI::TuranAPI_ENUMs Get_Resource_Type();
			virtual bool Verify_Resource_Data();
			Texture_Resource();
		};
	}
}

