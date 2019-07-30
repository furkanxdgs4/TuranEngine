#pragma once
#include "Includes.h"


#include "Graphics/GFX/Renderer/GFX_Shader_Resource.h"
#include "Graphics/GFX/Renderer/GFX_Shader.h"


class OGL3_Shader : public GFX_Shader {
public:
	static void Compile_All_Shaders();

	//There is a Compile_All_Shaders(), don't forget it!
	//Compile a shader program
	//This class is used to compile a specific shader when a shader editing tool is used!
	//So, this is a feature for future
	//For now, all shaders will be compiled at once when application starts!
	static void Compile_Shader(GFX_Shader* shader);

	OGL3_Shader(string name, string vertex_shader_path, string fragment_shader_path) : GFX_Shader(name, vertex_shader_path, fragment_shader_path) {

	}

};

void OGL3_Shader::Compile_All_Shaders() {
	for (GFX_Shader* shader : ALL_SHADERs) {
		Compile_Shader(shader);
		cout << "Compiled SHADER_ID: " << shader->PROGRAM_ID << endl;
	}
}

void OGL3_Shader::Compile_Shader(GFX_Shader* shader) {
	GLuint vertexShader_o, fragmentShader_o, shaderProgram;
	//If shader isn't compiled before!
	if (shader->Is_Compiled == false) {
		//Create shader objects to compile in them!
		vertexShader_o = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader_o = glCreateShader(GL_FRAGMENT_SHADER);;
		shaderProgram = glCreateProgram();

		shader->VERTEX_ID = vertexShader_o;
		shader->FRAGMENT_ID = fragmentShader_o;
		shader->PROGRAM_ID = shaderProgram;
	}
	//If shader is compiled before!
	else {
		shader->VERTEX_SOURCE = GFX_Shader::ReadShader(shader->VERTEX_DISK_PATH);
		shader->FRAGMENT_SOURCE = GFX_Shader::ReadShader(shader->FRAGMENT_DISK_PATH);

		vertexShader_o = shader->VERTEX_ID;
		fragmentShader_o = shader->FRAGMENT_ID;
		shaderProgram = shader->PROGRAM_ID;
	}

	//Compile vertex shader and set ID!
	const char* vertex_source = shader->VERTEX_SOURCE.c_str();
	glShaderSource(vertexShader_o, 1, &vertex_source, NULL);
	glCompileShader(vertexShader_o);
	shader->Is_Compiled = true;

	//Check compile issues!
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader_o, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader_o, 512, NULL, infolog);
		cout << vertex_source << endl << "Error: Vertex Shader couldn't compile!\n" << infolog << endl;
	}




	//Compile fragment shader and set ID!
	const char* fragment_source = shader->FRAGMENT_SOURCE.c_str();
	glShaderSource(fragmentShader_o, 1, &fragment_source, NULL);
	glCompileShader(fragmentShader_o);

	//Check compile issues!
	int frag_success;
	char frag_infolog[512];
	glGetShaderiv(fragmentShader_o, GL_COMPILE_STATUS, &frag_success);
	if (!frag_success) {
		glGetShaderInfoLog(fragmentShader_o, 512, NULL, frag_infolog);
		cout << fragment_source << endl << "Error: Fragment Shader couldn't compile!\n" << frag_infolog << endl;
	}


	//Link Vertex and Fragment Shader to Shader Program and set ID
	glAttachShader(shaderProgram, vertexShader_o);
	glAttachShader(shaderProgram, fragmentShader_o);
	glLinkProgram(shaderProgram);

	//Check linking issues
	int link_success;
	char link_infolog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &link_success);
	if (!link_success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, link_infolog);
		cout << "Error: Shader Program couldn't link!\n" << link_infolog << endl;
	}
}

OGL3_Shader First_Shader("First_Shader", "Source/Graphics/OPENGL3/Shaders/Main.vert", "Source/Graphics/OPENGL3/Shaders/Main.frag");
OGL3_Shader PostProcess_Shader("PostProcess_Shader", "Source/Graphics/OPENGL3/Shaders/Post_Process.vert", "Source/Graphics/OPENGL3/Shaders/Post_Process.frag");