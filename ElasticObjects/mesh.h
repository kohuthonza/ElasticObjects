#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "plane.h"

typedef struct Vertex {
public:
	Vertex(const glm::vec3 &_pos, const glm::vec3 &_color) {
		pos = _pos;
		color = _color;
	}

	inline glm::vec3*GetPos() { return &pos; }
	inline glm::vec3*GetColor() { return &color; }

private:
	glm::vec3 pos;
	glm::vec3 color;
};


class Mesh {
public:
	Mesh(	Vertex *vertices, unsigned int numVertices, 
			GLushort *indices, unsigned int numIndices,
			glm::vec3 *offsets, unsigned int numInstances);

	void Draw();

	void Update(glm::vec3 *offsets, const int numOffsets, const Plane &pl);

	virtual ~Mesh();

private:
	bool CheckCollision(const AABB &a, const AABB &b);

	enum {
		POSITION_VB,
		COLOR_VB,
		INDICES_VB,
		OFFSET_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];

	unsigned int drawVertexCount;
	unsigned int drawInsancesCount;

	glm::vec3 *instacePositionsPointer;

};