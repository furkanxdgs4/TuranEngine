#include "OGL4_APICommands.h"
#include "OpenGL4Source/OGL4_Display.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OpenGL4 {
	void APICommander::Swap_WindowBuffers_Immediately(GFX_API::WINDOW* window) {
		GLFWwindow* Window_Context = ((OpenGL4::WINDOW*)window)->GLFWWINDOW;
		glfwSwapBuffers(Window_Context);
	}

	void APICommander::Clear_WindowBuffer_Immediately(GFX_API::WINDOW* window, vec3 Clear_Color) {
		GLFWwindow* Window_Context = ((OpenGL4::WINDOW*)window)->GLFWWINDOW;
		glfwMakeContextCurrent(Window_Context);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(Clear_Color.x, Clear_Color.y, Clear_Color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}