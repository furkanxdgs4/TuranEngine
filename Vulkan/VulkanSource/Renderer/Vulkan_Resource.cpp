
#include "Vulkan_Resource.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"


namespace Vulkan {
	ShaderPipeline::ShaderPipeline() : ShaderStageCreateInfos(LASTUSEDALLOCATOR, 1, 2), ShaderModules(LASTUSEDALLOCATOR, 1, 2) {

	}
}