#include "vertex.h"

Vertex::Vertex(vec3 $pos, vec3 $vel, vec3 $force, float $mass) {
	pos = $pos;
	vel = $vel;
	force = $force;
	mass = $mass;
}

void Vertex::ApplyForce(vec3 $force) {
	force += $force;
}
