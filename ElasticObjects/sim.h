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

	void Draw();

	void Operate(float dt);

	void ResolveCollison();

	void RecomputeBoundingBox();

	void ToggleStop() { stop = !stop; }

	std::vector<Object *> &GetObjects() { return objects; }

private:
	std::vector<Object *> objects;

	bool IsInCollision(AABB *cube1, AABB *cube2);

	bool stop = false;
};