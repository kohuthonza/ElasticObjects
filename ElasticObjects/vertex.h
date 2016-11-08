#pragma once
#include <glm\glm.hpp>

using glm::vec3;

struct Vertex
{
	Vertex(vec3 $pos, vec3 $vel, vec3 $force, float $mass);

	vec3 pos;
	vec3 vel;
	vec3 force;
	float mass;

	void ApplyForce(vec3 $force);
};
