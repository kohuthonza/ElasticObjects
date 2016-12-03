#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "vertex.h"
#include "spring.h"

using glm::vec3;

typedef struct AABB {
	vec3 min;
	vec3 max;
};

class Object {
public:
	Object() {};

	Object(glm::vec3 normal, glm::vec3 point1, glm::vec3 point2, float size);

	void AddVertex(Vertex *vertex);

	void AddIndex(GLushort index);

	void AddSpring(int point1, int point2, float springConstat);

	void InitTest();

	void InitTestHexa();

	void InitOBJTest(glm::vec3 offset, glm::vec3 initialVel );

	void Solve();

	void Simulate(float dt);

	void Draw();

	void GenerateBoundingBox();

	void ResolveVertices(Object *other);

	std::vector<Vertex *> &GetVerts() { return verts; }

	AABB *GetAABB() { return &aabbCoords; }

	void CalculateBodyVolume();

	bool SpringExists(int point1, int point2);

	void GenerateSprings();

	bool IsInside(glm::vec3 point);

	bool VectorIntersectsTriangle(/* VECTOR: */ glm::vec3 Origin, glm::vec3 Dir, /* TRIANGLE: */ glm::vec3 TA, glm::vec3 TB, glm::vec3 TC);

	glm::vec3 CalculateSurfaceNormal(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3);

private:
	AABB aabbCoords;

	bool isPlane = false;
	vec3 normal;
	vec3 pointOnPlane;

	float BodyVolume;

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