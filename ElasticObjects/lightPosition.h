#pragma once
#include <glm\glm.hpp>

class LightPosition
{
public:
	LightPosition(glm::vec3 &pos);
	glm::vec3& GetPos() { return position; }
private:
	glm::vec3 position;

};

