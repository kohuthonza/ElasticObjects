#version 150

out vec4 outColor;
in vec3 inColor;
in vec3 inNormal;
in vec3 inPosition;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
 
void main(void) {    
	
	vec3 lightVector = normalize(lightPosition - inPosition);
	float diffuseBrightness =  clamp(dot(lightVector, normalize(inNormal)), 0, 1);

	vec3 brightness = clamp(ambientLight + diffuseBrightness, 0, 1);

	outColor = vec4(brightness * inColor, 1.0f);
}
