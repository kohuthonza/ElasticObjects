#version 150

out vec4 outColor;
in vec3 inColor;

 
void main(void) {    
	outColor = vec4(inColor, 1.0f);
}
