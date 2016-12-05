#version 150

out vec4 outColor;
in vec3 inColor;
in vec3 inNormal;
in vec3 inPosition;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
 
void main(void) {    
	
	vec3 lightVector = normalize(lightPosition - inPosition);
	float diffuseBrightness =  clamp(dot(lightVector, normalize(inNormal)), 0, 1);

	vec3 reflectedLightVector = reflect(-lightVector, normalize(inNormal));
	vec3 eyeVector = normalize(eyePosition - inPosition);
	float s = dot( eyeVector, reflectedLightVector);
	float specularity = clamp(pow(s, 45), 0, 1);

	vec3 brightness = clamp(ambientLight + diffuseBrightness + specularity, 0, 1);

	outColor = vec4(brightness * inColor, 1.0f);
}
