#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>

#include <vector>

typedef struct AABB {
	glm::vec3 min;
	glm::vec3 max;
} AABB;

class Plane {
public:
	Plane(const glm::vec3 & color, float planeHeight = -0.5f);

	void Draw();

	AABB GetAABB() const { return AABB{ min, max }; }

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

