#include "sim.h"

#include <iostream>

Sim::Sim() {
	
	//objects.push_back(new Object(glm::vec3(-1, 0, 0), glm::vec3(3, -2.5, -1), glm::vec3(3, -4.5, -1), 5.f));
	//objects.back()->GenerateBoundingBox();
	
	//objects.push_back(new Object());
	//objects.back()->InitOBJTest("obj_models\\plane.obj", 1.1f, glm::vec3(2, 2, 0), glm::vec3(-3, 0, 0));
	//objects.back()->GenerateBoundingBox();

	objects.push_back(new Object());
	objects.back()->InitOBJTest("obj_models\\sphere2.obj", 1.1f, 20.0f, glm::vec3(-2, 2, 0), glm::vec3(0, 0, 0));
	objects.back()->GenerateBoundingBox();

	objects.push_back(new Object());
	objects.back()->InitOBJTest("obj_models\\bunny_reduced_600faces.obj", 1.1f, 20.0f, glm::vec3(-2, 5, 0), glm::vec3(0, 0, 0));
	objects.back()->GenerateBoundingBox();

	objects.push_back(new Object(glm::vec3(0, 1, 0), glm::vec3(0, -6.5, -1), glm::vec3(3, -6.5, -1), 5.f));
	objects.back()->GenerateBoundingBox();

	objects.push_back(new Object(glm::vec3(0, 0, 1), glm::vec3(0, 2.5, -2), glm::vec3(3, 2.5, -2), 5.f));
	objects.back()->GenerateBoundingBox();
	
	
	objects.push_back(new Object());
	objects.back()->InitOBJTest("obj_models\\plane1.obj", 0.1f, 20.0f, glm::vec3(5, 0, 0), glm::vec3(0, 0, 0));
	objects.back()->GenerateBoundingBox();
	
}

Sim::~Sim(){
	for (unsigned int i = 0; i < objects.size(); ++i)
		delete objects[i];
}

void Sim::Solve() {
	for (unsigned int a = 0; a < objects.size(); ++a)
		objects[a]->Solve();
}

void Sim::Simulate(float dt) {
	for (unsigned int a = 0; a < objects.size(); ++a)                // We Will Iterate Every Mass.
		objects[a]->Simulate(dt);
}

void Sim::Draw(GLuint program, glm::vec3 cameraPosition, glm::vec3 lightPosition) {
	for (unsigned int a = 0; a < objects.size(); ++a)
		objects[a]->Draw(program, cameraPosition, lightPosition);
}

void Sim::Operate(float dt, GLuint program, glm::vec3 cameraPosition, glm::vec3 lightPosition) {
	init();		
	if (!stop) {	
		ResolveCollison();
		Solve();		
		Simulate(dt);		
		RecomputeBoundingBox();
	}
	Draw(program, cameraPosition, lightPosition);
}

void Sim::ResolveCollison(){
	for (unsigned int i = 0; i < objects.size(); ++i) {
		for (unsigned int j = i + 1; j < objects.size(); ++j) {			
			auto object1AABB = objects[i]->GetAABB();
			auto object2AABB = objects[j]->GetAABB();	

			if (IsInCollision(object1AABB, object2AABB)) {
				objects[i]->ResolveVertices(objects[j]);
			}
		}
	}
}

void Sim::RecomputeBoundingBox(){
	for (unsigned int a = 0; a < objects.size(); ++a)
		objects[a]->GenerateBoundingBox();
}

inline bool Sim::IsInCollision(AABB * cube1, AABB * cube2){	
	// Exit with no intersection if found separated along an axis
	if (cube1->max.x < cube2->min.x || cube1->min.x > cube2->max.x) return false;

	if (cube1->max.y < cube2->min.y || cube1->min.y > cube2->max.y) return false;

	if (cube1->max.z < cube2->min.z || cube1->min.z > cube2->max.z) return false;

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}
