#include "spring.h"

using glm::vec3;

Spring::Spring(unsigned int _index1, unsigned int _index2, Vertex * $point1, Vertex * $point2, float springLen, float springConst) {
	index1 = _index1;
	index2 = _index2;

	point1 = $point1;
	point2 = $point2;

	springLength = springLen;
	springConstant = springConst;
}

void Spring::Solve() {
	vec3 springVector = point1->pos - point2->pos;

	float r = glm::length(springVector);

	vec3 force(0.0f, 0.0f, 0.0f);

	if (r != 0.0f)
		force += -(springVector / r) * (r - springLength) * springConstant;

	force += -(point1->vel - point2->vel)*springFrictionConstant;
	point1->ApplyForce(force);
	point2->ApplyForce(-force);
}