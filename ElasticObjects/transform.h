#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform {
public:
	Transform::Transform(const glm::vec3 &_pos = glm::vec3(), glm::vec3 &_rot = glm::vec3(), glm::vec3 &_scale = glm::vec3(1.0f, 1.0f, 1.0f)) : 
		pos(_pos), rot(_rot), scale(_scale) {}

	inline glm::mat4 GetModel() const {
		glm::mat4 posMatrix = glm::translate(pos);		

		glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));

		glm::mat4 scaleMatrix = glm::scale(scale);

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}

	inline glm::vec3 &GetPos() { return pos; }
	inline glm::vec3 &GetRot() { return rot; }
	inline glm::vec3 &GetScale() { return scale; }

	inline void SetPos(glm::vec3 &_pos) { pos = _pos; }
	inline void SetRot(glm::vec3 &_rot) { pos = _rot; }
	inline void SetScale(glm::vec3 &_scale) { pos = _scale; }

	//virtual ~Transform();
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};