#include "Texture_Resource.h"

using namespace TuranAPI::File_System;

Texture_Properties::Texture_Properties(){}
Texture_Properties::Texture_Properties(TuranAPI::TuranAPI_ENUMs dimension, TuranAPI::TuranAPI_ENUMs mipmap_filtering, TuranAPI::TuranAPI_ENUMs wrapping, TuranAPI::TuranAPI_ENUMs channel_type, TuranAPI::TuranAPI_ENUMs value_type)
	: DIMENSION(dimension), MIPMAP_FILTERING(mipmap_filtering), WRAPPING(wrapping), CHANNEL_TYPE(channel_type), VALUE_TYPE(value_type) {}


Texture_Resource::Texture_Resource() {
	ALL_TEXTUREs.push_back(this);
}

vector<Texture_Resource*> Texture_Resource::ALL_TEXTUREs = vector<Texture_Resource*>{};

TuranAPI::TuranAPI_ENUMs Texture_Resource::Get_Resource_Type() {
	return TuranAPI_ENUMs::TEXTURE_RESOURCE;
}

bool Texture_Resource::Verify_Resource_Data() {
	if (NAME != "" && PATH != "") {
		if (WIDTH > 0 && HEIGHT > 0) {
			if (DATA_SIZE > 0 && DATA != nullptr) {
				if (Properties.DIMENSION != TuranAPI_ENUMs::API_TEXTURE_2D) {
					TuranAPI::Breakpoint("Texture Resource isn't verified because it's dimension is something unsupported!\n" +
						string("It has a different Enum value than API_TEXTURE_2D"));
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
	TuranAPI::Breakpoint("Texture Resource isn't verified because it has invalid Name or Path!"
		+ string("NAME: ") + NAME + string("\n PATH: ") + PATH + string("\n"));
	return false;
}