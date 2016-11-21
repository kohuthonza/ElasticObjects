#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>

#include <vector>

class Plane {
public:
	Plane(const glm::vec3 & color, float planeHeight);

	Plane(glm::vec3 normal, glm::vec3 point1, glm::vec3 point2, float size);

	void Draw();

	~Plane();

private:	
	const GLfloat size = 50.0f;

	glm::vec3 min;
	glm::vec3 max;

	enum {
		POSITION_VB,
		COLOR_VB,
		INDICES_VB,
		
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
};

