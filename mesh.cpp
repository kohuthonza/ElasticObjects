#include "mesh.h"
#include <vector>
#include <iostream>

Mesh::Mesh(Vertex *vertices, unsigned int numVertices, GLushort *indices, unsigned int numIndices, glm::vec3 *offsets, unsigned int numInstances){
	drawVertexCount = numIndices;
	drawInsancesCount = numInstances;
	instacePositionsPointer = offsets;

	glGenVertexArrays(3, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;

	positions.reserve(numVertices);
	colors.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i) {
		positions.push_back(*vertices[i].GetPos());
		colors.push_back(*vertices[i].GetColor());
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
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(offsets[0]) * 3, offsets, GL_DYNAMIC_COPY);

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

	instacePositionsPointer[0].x += 0.0000005f;
	glBufferSubData(vertexArrayBuffers[OFFSET_VB], 0, drawInsancesCount * sizeof(instacePositionsPointer[0]) * 3, instacePositionsPointer);

	glDrawElementsInstanced(GL_TRIANGLES, drawVertexCount, GL_UNSIGNED_SHORT, 0, drawInsancesCount);
	glBindVertexArray(0);
}

Mesh::~Mesh(){
	glDeleteVertexArrays(1, &vertexArrayObject);
}
