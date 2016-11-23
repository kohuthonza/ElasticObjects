﻿#include "object.h"
#include "OBJ_Loader.h"
#include <iostream>
#include <random>
#include <chrono>

float TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	glm::vec3 VectorAB = A - B;
	glm::vec3 VectorAC = A - C;

	glm::vec3 CrossProduct = glm::cross(VectorAB, VectorAC);
	
	return 0.5 * glm::length(CrossProduct);
}


void Object::CalculateBodyVolume() {
	/*
	* For more information refer to Gauss theorem - "with closed shapes we
	*	are able to replace integration over volume by integration
	*	over surface of the body".
	*/


	/*
	* Number of edges? (refer to eulers formula)
	* 3 faces = 2 edges 
	* E = 3/2 F = 3/2 * |indices|/3
	*/

}



Object::Object(glm::vec3 $normal, glm::vec3 point1, glm::vec3 point2, float D){
	isPlane = true;
	normal = $normal;
	pointOnPlane = point1;

	glm::vec3 tangent = glm::normalize(point2 - point1);
	glm::vec3 bitangent = glm::cross(tangent, normal);	

	vertices.push_back(point1 - tangent*D - bitangent*D);
	vertices.push_back(point1 + tangent*D - bitangent*D);
	vertices.push_back(point1 + tangent*D + bitangent*D);
	vertices.push_back(point1 - tangent*D + bitangent*D);

	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<float> unif(0, 1);

	vec3 color( unif(rng), unif(rng), unif(rng));

	verts.push_back(new Vertex(vertices[0], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[1], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[2], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[3], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


inline void Object::AddVertex(Vertex * vertex) {
	verts.push_back(vertex);
}

inline void Object::AddIndex(GLushort index) {
	indices.push_back(index);
}

inline void Object::AddSpring(int point1, int point2, float springConstat) {
	springs.push_back(new Spring(verts[point1], verts[point2], glm::distance(verts[point2]->pos, verts[point1]->pos), springConstat));
}

void Object::InitOBJTest() {

	OBJ_Loader obj("obj_models\\simple_sphere.obj");
	vertices = obj.getVertices();
	indices = obj.getIndices();
	normals = obj.getNormals();

	//std::cout << "Normals size: " << normals.size() << std::endl;
	//std::cout << "Indices size: " << indices.size() << std::endl;

	for (auto &i : vertices) {
		verts.push_back(new Vertex( i, vec3(0.0, -2.0, 0.0), vec3(0.0, 0.0, 0.0), 1.1f ) );
	}
	
	for (unsigned int i = 0; i < indices.size(); i += 3) { // loop over all faces
		unsigned int vert_index[3];

		vert_index[0] = indices[i];
		vert_index[1] = indices[i + 1];
		vert_index[2] = indices[i + 2];
		

	}
	for (unsigned int i = 0; i < obj.getVerticesNumber(); i++) {
		for (unsigned int j = 0; j < obj.getVerticesNumber(); j++ ) {
			AddSpring(i, j, 100.2f);
		}
	}
	
 	for (unsigned int i = 0; i < obj.getVerticesNumber(); i++) {
		colors.push_back(red);
	}
	
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}



void Object::InitTest() {

	verts.push_back(new Vertex(vec3(-1.0, -1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, -1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));


	vertices.push_back(verts[0]->pos);
	vertices.push_back(verts[1]->pos);
	vertices.push_back(verts[2]->pos);
	vertices.push_back(verts[3]->pos);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	AddSpring(0, 1, 1000.0f);
	AddSpring(1, 2, 1000.0f);
	AddSpring(3, 2, 1000.0f);
	AddSpring(0, 3, 1000.0f);
	AddSpring(0, 2, 1000.0f);
	AddSpring(1, 3, 1000.0f);

	colors.push_back(red);
	colors.push_back(red);
	colors.push_back(red);
	colors.push_back(red);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Object::InitTestHexa() {
	verts.push_back(new Vertex(vec3(-0.25, 3.0, -1.0), vec3(0.4, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 2.0f));
	verts.push_back(new Vertex(vec3(0.25, 3.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 3.75, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 4.25, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(0.25, 5.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-0.25, 5.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 4.25, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 3.75, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));

	vertices.push_back(verts[0]->pos);
	vertices.push_back(verts[1]->pos);
	vertices.push_back(verts[2]->pos);
	vertices.push_back(verts[3]->pos);
	vertices.push_back(verts[4]->pos);
	vertices.push_back(verts[5]->pos);
	vertices.push_back(verts[6]->pos);
	vertices.push_back(verts[7]->pos);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(4);

	indices.push_back(0);
	indices.push_back(6);
	indices.push_back(5);

	indices.push_back(0);
	indices.push_back(7);
	indices.push_back(6);

	AddSpring(0, 6, 100.0f);
	AddSpring(0, 5, 100.0f);
	AddSpring(0, 4, 100.0f);
	AddSpring(0, 3, 100.0f);

	AddSpring(1, 7, 100.0f);
	AddSpring(1, 6, 100.0f);
	AddSpring(1, 5, 100.0f);
	AddSpring(1, 4, 100.0f);

	AddSpring(2, 7, 1.0f);
	AddSpring(2, 6, 1.0f);
	AddSpring(2, 5, 1.0f);
	AddSpring(2, 0, 1.0f);

	AddSpring(0, 1, 1.0f);
	AddSpring(1, 2, 1.0f);
	AddSpring(2, 3, 1.0f);
	AddSpring(3, 4, 1.0f);
	AddSpring(4, 5, 1.0f);
	AddSpring(5, 6, 1.0f);
	AddSpring(6, 7, 1.0f);
	AddSpring(7, 0, 1.0f);

	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Object::Solve() {
	if (isPlane)
		return;

	for (unsigned int i = 0; i < springs.size(); ++i) {
		springs[i]->Solve();
	}
}

void Object::Simulate(float dt) {
	if (isPlane)
		return;

	/*
	* Calculate and accumulate gravity and spring forces
	*	for all particles.
	*/
	for (unsigned int i = 0; i < verts.size(); ++i) {
		verts[i]->ApplyForce(gravitation * verts[i]->mass);
		verts[i]->ApplyForce(-verts[i]->vel * airFrictionConstant);
	}
	/*
	*  Simulate ideal gas pressure from the inside of the object
	*/
	for (unsigned int i = 0; i < indices.size(); i+=3) { // loop over all faces
		glm::vec3 v[3];
		
		v[0] = vertices[indices[ i ]];
		v[1] = vertices[indices[ i + 1 ]];
		v[2] = vertices[indices[ i + 2 ]];

		//Calculate the pressure value
		const float Na = 6.02214e23; //- Avogardo number
		const float kb = 1.380648e-23; //- Bolzman konstant
		float Temperature = 295.0; //- Kelvin temperature
		
		float R = Na * kb;

		//float Force = n R T Ve-1
		glm::vec3 GasPressureForce = normals[i / 3] * R * Temperature / Volume;

		//Loop over particles which define the face

		//Multiply result by field of the face and ˆn
		float Area = TriangleArea(v[0], v[1], v[2]);

		//Accumulate finally pressure force to the particle.

	}


	for (unsigned int i = 0; i < verts.size(); ++i) {
		verts[i]->vel += (verts[i]->force / verts[i]->mass) * dt;
		vec3 newPosVector = verts[i]->vel * dt;
		verts[i]->pos += newPosVector;

		vertices[i] = verts[i]->pos;

		verts[i]->force = vec3(0, 0, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
}

void Object::Draw() {
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Object::GenerateBoundingBox(){
	//std::cout << "generating BB's" << std::endl;
	vec3 tempMin = vertices[0];
	vec3 tempMax = vertices[0];

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		if (vertices[i].x < tempMin.x) tempMin.x = vertices[i].x;
		if (vertices[i].y < tempMin.y) tempMin.y = vertices[i].y;
		if (vertices[i].z < tempMin.z) tempMin.z = vertices[i].z;

		if (vertices[i].x > tempMax.x) tempMax.x = vertices[i].x;
		if (vertices[i].y > tempMax.y) tempMax.y = vertices[i].y;
		if (vertices[i].z > tempMax.z) tempMax.z = vertices[i].z;
	}
	
	aabbCoords.max = tempMax;
	aabbCoords.min = tempMin;
}

void Object::ResolveVertices(Object * other) {
	
	if (isPlane || other->isPlane) {
		Object *planeObject;
		Object *generalObject;
		if (isPlane) {
			planeObject = this;
			generalObject = other;
		}
		else {
			planeObject = other;
			generalObject = this;
		}
		

		for (unsigned int i = 0; i < generalObject->GetVerts().size(); ++i) {
			float distance = glm::dot((generalObject->GetVerts()[i]->pos - planeObject->pointOnPlane), planeObject->normal);

			if (abs(distance) < 0.5f) {				
				vec3 rv = -generalObject->GetVerts()[i]->vel;
				float velAlongNormal = glm::dot(rv, planeObject->normal);
				float e = 0.1f;

				float j = -(1.0f - e) * velAlongNormal;
				j /= 1 / generalObject->GetVerts()[i]->mass;

				vec3 impulse = j * planeObject->normal;
				generalObject->GetVerts()[i]->vel -= (1.0f / generalObject->GetVerts()[i]->mass) * impulse;
			}
		}		
	}
	else {
		for (unsigned int i = 0; i < verts.size(); ++i) {
			for (unsigned int j = 0; j < other->GetVerts().size(); ++j) {
				float dist = glm::distance(verts[i]->pos, other->GetVerts()[j]->pos);
				if (dist <= 1.f) {
					//std::cout << "vertex " << i << " and " << j << " are in col" << std::endl;
				}
			}
		}
	}
}

