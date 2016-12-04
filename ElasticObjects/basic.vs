#version 150

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;

out vec3 inColor;
out vec3 inNormal;
out vec3 inPosition;
 
uniform mat4 transform;

void main(void) {
    gl_Position = transform * vec4(position,  1.0);
	inColor = color;
	inNormal = normal;
	inPosition = position;
}
