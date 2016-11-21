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

	void Operate(float dt, int stop);

	void ResolveCollison();

	void RecomputeBoundingBox();

private:
	std::vector<Object *> objects;

	bool IsInCollision(AABB *cube1, AABB *cube2);
};