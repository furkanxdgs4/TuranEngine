#pragma once
#include "VulkanSource/Vulkan_ENUMs.h"
#include "GFXSource/Renderer/GFX_Resource.h"


namespace Vulkan {
	struct VK_API Texture : public GFX_API::Texture {
		VkImage Image = {};
		VkImageView ImageView = {};

	};

	struct VK_API ShaderPipeline {
		ShaderPipeline();
		GFX_API::Material_Type_Resource* MatType_Resource = nullptr;
		Vector<VkPipelineShaderStageCreateInfo> ShaderStageCreateInfos;
		Vector<VkShaderModule> ShaderModules;
	};

}