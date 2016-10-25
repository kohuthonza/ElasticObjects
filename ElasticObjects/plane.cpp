#include "plane.h"

Plane::Plane(const glm::vec3 &color){
	glm::vec3 points[] = {
		glm::vec3(0 - size/2, -10, 0 -10),
		glm::vec3(size - size/2, -10, -10),
		glm::vec3(size - size/2, -10, size - 10),
		glm::vec3(0 - size/2, -10, size - 10), 
	};

	GLuint indices[] = {
		0, 1, 3, 
		1, 2, 3
	};

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;

	positions.reserve(4);
	colors.reserve(4);

	for (int i = 0; i < 4; ++i) {
		positions.push_back(points[i]);
		colors.push_back(color);
	}

	min = points[0];
	max = points[2];

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices[0]), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Plane::Draw(){
	glBindVertexArray(vertexArrayObject);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Plane::~Plane(){
	glDeleteVertexArrays(NUM_BUFFERS, &vertexArrayObject);
}
