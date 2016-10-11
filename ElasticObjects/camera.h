#pragma once
#include <glm\glm.hpp>
#include <glm\gtx/transform.hpp>

class Camera {
public:
	Camera(glm::vec3 &pos, float fov, float aspect, float zNear, float zFar);

	inline glm::mat4 GetViewProjection() const {
		return perspective * glm::lookAt(position, position + forward, up);
	}

	glm::vec3& GetPos() { return position; }
	glm::vec3 &GetFwd() { return forward; }
	const glm::vec3 &GetUp() { return up; }

private:
	glm::mat4 perspective;
	glm::vec3 position;
	glm::vec3 forward;
	const glm::vec3 up;
};