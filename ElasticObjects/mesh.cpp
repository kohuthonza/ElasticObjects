#include "mesh.h"

#include <vector>
#include <iostream>

Mesh::Mesh(
	Vertex *vertices, unsigned int numVertices, 
	GLushort *indices, unsigned int numIndices, 
	glm::vec3 *offsets, unsigned int numInstances)
{
	drawVertexCount = numIndices;
	drawInsancesCount = numInstances;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> offsetsVec;

	positions.reserve(numVertices);
	colors.reserve(numVertices);
	offsetsVec.reserve(numInstances);

	for (unsigned int i = 0; i < numVertices; ++i) {
		positions.push_back(*vertices[i].GetPos());
		colors.push_back(*vertices[i].GetColor());
	}

	for (unsigned int i = 0; i < numInstances; ++i) {
		offsetsVec.push_back( offsets[i*2]);
	}

	//positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//offsets
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(offsetsVec[0]), &offsetsVec[0], GL_DYNAMIC_COPY);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

void Mesh::Draw(){
	glBindVertexArray(vertexArrayObject);
	glDrawElementsInstanced(GL_TRIANGLES, drawVertexCount, GL_UNSIGNED_SHORT, 0, drawInsancesCount);
	glBindVertexArray(0);
}

void Mesh::Update(glm::vec3 * offsets, const int numOffsets, const Plane &pl){
	std::vector<glm::vec3> offsetsVec;
	offsetsVec.reserve(numOffsets);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[OFFSET_VB]);

	const AABB planeAABB = pl.GetAABB();

	glm::vec3 directionVector;

	for (int i = 0; i < numOffsets; ++i) {
		AABB cubeAABB = {
			glm::vec3(offsets[i*2].x, offsets[i*2].y - 1, offsets[i*2].z - 1),
			glm::vec3(offsets[i*2].x, offsets[i*2].y + 1 , offsets[i*2].z + 1),
		};

		directionVector = offsets[i * 2 + 1];

		if (CheckCollision(planeAABB, cubeAABB)) {
			directionVector.y = -directionVector.y;
		}	

		offsets[i*2] += directionVector;
		offsets[i * 2 + 1] = directionVector;
		offsetsVec.push_back(offsets[i * 2]);
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, numOffsets * sizeof(offsetsVec[0]), &offsetsVec[0]);

	glBindVertexArray(0);
}

Mesh::~Mesh(){
	glDeleteVertexArrays(NUM_BUFFERS, &vertexArrayObject);
}

bool Mesh::CheckCollision(const AABB & a, const AABB & b){
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;

	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}
