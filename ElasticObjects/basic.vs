#version 150

attribute vec3 position;
attribute vec3 color;

out vec3 inColor;
 
uniform mat4 transform;

void main(void) {
    gl_Position = transform * vec4(position,  1.0);
	inColor = color;
}
