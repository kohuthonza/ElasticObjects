#include "object.h"
#include "OBJ_Loader.h"
#include <iostream>

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

	colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

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
	//normals = obj.getNormals();

	for (auto &i : vertices) {
		verts.push_back(new Vertex( i, vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f ) );
	}
	
	for (unsigned int i = 0; i < obj.getVerticesNumber(); i++) {
		for (unsigned int j = 0; j < obj.getVerticesNumber(); j++ ) {
			AddSpring(i, j, 1.0f);
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

	AddSpring(0, 1, 100.0f);
	AddSpring(1, 2, 100.0f);
	AddSpring(3, 2, 100.0f);
	AddSpring(0, 3, 100.0f);
	AddSpring(0, 2, 100.0f);
	AddSpring(1, 3, 100.0f);

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

	for (unsigned int i = 0; i < verts.size(); ++i) {
		verts[i]->ApplyForce(gravitation * verts[i]->mass);
		verts[i]->ApplyForce(-verts[i]->vel * airFrictionConstant);

		verts[i]->vel += (verts[i]->force / verts[i]->mass) * dt;
		vec3 newPosVector = verts[i]->vel * dt;
		verts[i]->pos += newPosVector;

		vertices[i] = verts[i]->pos;

		verts[i]->force = vec3(0, 0, 0);

		/*if (verts[i]->pos.y < -3.5f) {
			vec3 v;
			v = verts[i]->vel;

			v.y = 0.0f;

			verts[i]->ApplyForce(-v * 0.2f);

			v = verts[i]->vel;
			v.x = 0;
			v.z = 0;

			if (v.y < 1.0f) {
				verts[i]->ApplyForce(-v* 2.0f);
			}

			vec3 force = vec3(0, 100.0f, 0) * (-3.5f - verts[i]->pos.y);

			verts[i]->ApplyForce(force);
		}*/

		//from here
		/*if (masses[a]->GetPos().y < groundHeight) {
		vec3 v;
		v = masses[a]->GetVel();

		v.y = 0;

		masses[a]->ApplyForce(-v * groundFrictionConstant);

		v = masses[a]->GetVel();
		v.x = 0;
		v.z = 0;

		if (v.y < 0)
		masses[a]->ApplyForce(-v*groundAbsorptionConstant);

		vec3 force = vec3(0, groundRepulsionConstant, 0) * (groundHeight - masses[a]->GetPos().y);

		masses[a]->ApplyForce(force);
		}*/
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
		if (vertices[i].z < tempMin.z) tempMin.z = vertices[i].y;

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
		std::cout << "plane col" << std::endl;

		for (unsigned int i = 0; i < generalObject->GetVerts().size(); ++i) {
			vec3 distance = (generalObject->GetVerts()[i]->pos - planeObject->pointOnPlane) * planeObject->normal;
			//std::cout << "vertex " << i << " dist " << distance.x << " " << distance.y << " " << distance.z << std::endl;

			if (distance.y < 0.5f) {
				vec3 v;
				v = generalObject->GetVerts()[i]->vel;

				v.y = 0.0f;

				generalObject->GetVerts()[i]->ApplyForce(-v * 0.2f);

				v = generalObject->GetVerts()[i]->vel;
				v.x = 0;
				v.z = 0;

				if (v.y < 1.0f) {
					generalObject->GetVerts()[i]->ApplyForce(-v* 2.0f);
				}

				vec3 force = vec3(0, 100.0f, 0) * (-3.5f - generalObject->GetVerts()[i]->pos.y);

				generalObject->GetVerts()[i]->ApplyForce(force);
			}
		}		
	}
	else {
		for (unsigned int i = 0; i < verts.size(); ++i) {
			for (unsigned int j = 0; j < other->GetVerts().size(); ++j) {
				float dist = glm::distance(verts[i]->pos, other->GetVerts()[j]->pos);
				if (dist <= 1.f) {
					std::cout << "vertex " << i << " and " << j << " are in col" << std::endl;
				}
			}
		}
	}
}

