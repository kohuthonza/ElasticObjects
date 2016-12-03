#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>


class OBJ_Loader
{
public:
	OBJ_Loader(const std::string filename);
	~OBJ_Loader();

	std::vector<glm::vec3> getVertices();
	std::vector<GLushort> getIndices();
	std::vector<glm::vec3> getNormals();
	unsigned int getVerticesNumber();
	void parseFace(std::string s, int *vertex, int *texture, int *normal);

private:
	std::vector<glm::vec3> vertices;
	std::vector<GLushort> indices;
	std::vector<glm::vec3> normals;
	unsigned int verticesNumber = 0;
};

