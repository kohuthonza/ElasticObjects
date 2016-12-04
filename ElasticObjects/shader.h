#pragma once
#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "transform.h"
#include "camera.h"

class Shader
{
public:
	Shader(const std::string &fileName);

	void Bind();
	GLuint GetProgram();
	void Update(const Transform &transform, const Camera& camera);

	virtual ~Shader();

private:
	static const unsigned int NUM_SHADERS = 2;

	enum {
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint program;
	GLuint shaders[NUM_SHADERS];

	GLuint uniforms[NUM_UNIFORMS];

};