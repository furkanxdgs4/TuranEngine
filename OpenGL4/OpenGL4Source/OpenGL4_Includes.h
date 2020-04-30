#pragma once
#include "TuranAPI/Logger/Logger_Core.h"
#include "TuranAPI/Profiler/Profiler_Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef WINDOWS10_FORENGINE
#ifdef OPENGL4_BUILD
#define OGL4_API __declspec(dllexport)
#else
#define OGL4_API __declspec(dllimport)
#endif // OPENGL4_BUILD
#endif // WINDOWS10_FORENGINE
