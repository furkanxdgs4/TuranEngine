#include "Texture_Resource.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"
#include <flatbuffers/flatbuffers.h>
#include "GFXSource/GFX_FileSystem/DataFormats/GFXAPI_Resources_generated.h"

namespace GFX_API {
	Texture_Properties::Texture_Properties() {}
	Texture_Properties::Texture_Properties(TEXTURE_DIMENSIONs dimension, TEXTURE_MIPMAPFILTER mipmap_filtering, TEXTURE_WRAPPING wrapping, TEXTURE_CHANNELs channel_type, UNIFORMTYPE value_type)
		: DIMENSION(dimension), MIPMAP_FILTERING(mipmap_filtering), WRAPPING(wrapping), CHANNEL_TYPE(channel_type), VALUE_TYPE(value_type) {}

	Texture_Resource::Texture_Resource() {

	}


}