#include "shader.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage);
static std::string LoadShader(const std::string &fileName);

static GLuint CreateShader(const std::string &text, GLenum shaderType);

Shader::Shader(const std::string & fileName){
	program = glCreateProgram();
	shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);	// Vertex shader
	shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);	// Fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
		glAttachShader(program, shaders[i]);
	}

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "color");

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Link failed: ");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Validation failed: ");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");	
}

void Shader::Bind(){
	glUseProgram(program);
}

GLuint Shader::GetProgram() {
	return program;
}

void Shader::Update(const Transform & transform, const Camera &camera){
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();

	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

Shader::~Shader(){
	for (unsigned int i = 0; i < NUM_SHADERS; ++i) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}	
	glDeleteProgram(program);
}

static GLuint CreateShader(const std::string &text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType );

	if (shader == 0) {
		std::cerr << "Shader creation failed" << std::endl;
	}

	const GLchar *shaderSourceStrings[1];
	GLint shadeSourceStringLengths[1];
	shadeSourceStringLengths[0] = text.length();
	shaderSourceStrings[0] = text.c_str();

	glShaderSource(shader, 1, shaderSourceStrings, shadeSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Shader compilation failed: ");

	return shader;
}


std::string LoadShader(const std::string &fileName) {
	std::ifstream file;

	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

