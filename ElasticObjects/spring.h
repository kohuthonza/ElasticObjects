#pragma once

#include "vertex.h"

using glm::vec3;

class Spring {
private:
	unsigned int index1;
	unsigned int index2;

	Vertex *point1;
	Vertex *point2;

	float springConstant;
	float springLength;

	const float springFrictionConstant = 0.6f;

public:
	Spring(unsigned int _index1, unsigned int _index2, Vertex *$point1, Vertex *$point2, float springLen, float springConst);

	unsigned int Index1() { return index1; };
	unsigned int Index2() { return index2; };
	void Solve();
};