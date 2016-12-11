#pragma once

#include <vector>

#include "object.h"

class Sim {
public:
	Sim();

	~Sim();

	void Operate(float dt, GLuint program, glm::vec3 cameraPosition, glm::vec3 lightPosition);
	
	void ToggleStop() { stop = !stop; }

	std::vector<Object *> &GetObjects() { return objects; }

private:
	std::vector<Object *> objects;

	bool IsInCollision(AABB *cube1, AABB *cube2);

	bool stop = false;

	void Solve();

	void Simulate(float dt);

	void Draw(GLuint program, glm::vec3 cameraPosition, glm::vec3 lightPosition);

	void ResolveCollison();

	void RecomputeBoundingBox();

	void IsTooFar();
};