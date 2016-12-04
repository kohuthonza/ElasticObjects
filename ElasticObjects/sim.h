#pragma once

#include <vector>

#include "object.h"

class Sim {
public:
	Sim();

	~Sim();

	void init() {};

	void Solve();

	void Simulate(float dt);

	void Draw(GLuint program, glm::vec3 lightPosition);

	void Operate(float dt, GLuint program, glm::vec3 lightPosition);

	void ResolveCollison();

	void RecomputeBoundingBox();

	void ToggleStop() { stop = !stop; }

	std::vector<Object *> &GetObjects() { return objects; }

private:
	std::vector<Object *> objects;

	bool IsInCollision(AABB *cube1, AABB *cube2);

	bool stop = false;
};