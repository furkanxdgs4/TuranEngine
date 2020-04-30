#pragma once
#include "TuranAPI/TuranAPI_Core.h"
#include "GFXSource/GFX_Core.h"

#ifdef WINDOWS10_FORENGINE
#ifdef VULKAN_BUILD_TURAN
#define VK_API __declspec(dllexport)
#else
#define VK_API __declspec(dllimport)
#endif // VULKAN_BUILD_TURAN
#endif // WINDOWS10_FORENGINE
