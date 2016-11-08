#pragma once

#include "vertex.h"

using glm::vec3;

class Spring {
private:
	Vertex *point1;
	Vertex *point2;

	float springConstant;
	float springLength;

	const float airFrictionConstant = 0.2f;

public:
	Spring(Vertex *$point1, Vertex *$point2, float springLen, float springConst);

	void Solve();
};