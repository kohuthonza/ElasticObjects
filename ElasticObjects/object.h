#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "vertex.h"
#include "spring.h"

using glm::vec3;

class Object {
public:
	Object() {};

	void AddVertex(Vertex *vertex);

	void AddIndex(GLushort index);

	void AddSpring(int point1, int point2, float springConstat);

	void InitTest();

	void InitTestHexa();

	void InitOBJTest();

	void Solve();

	void Simulate(float dt);

	void Draw();

private:
	const vec3 gravitation = vec3(0, -9.81f, 0);
	const float airFrictionConstant = 0.2f;

	std::vector<Vertex *> verts;
	std::vector<Spring *> springs;

	std::vector<glm::vec3> vertices;
	std::vector<GLushort> indices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;

	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);

	enum {
		POSITION_VB,
		COLOR_VB,
		INDICES_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
};