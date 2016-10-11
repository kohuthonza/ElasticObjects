#include "camera.h"

Camera::Camera(glm::vec3 &pos, float fov, float aspect, float zNear, float zFar) : up(glm::vec3(0.0, 1.0, 0.0)){
	 perspective = glm::perspective(fov, aspect, zNear, zFar);
	 position = pos;
	 forward = glm::vec3(0.0f, 0.0f, -1.0f);
}