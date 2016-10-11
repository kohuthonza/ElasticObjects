#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

#include <GL/glew.h>


#define WIDTH 800
#define HEIGHT 600


#undef main

int main(void) {
	Display display(WIDTH, HEIGHT, "OpenGL Test");
	Shader shader("basic");

	glm::vec3 red(1.0f, 0.0f, 0.0f);	//red
	glm::vec3 blue(0.0f, 0.0f, 1.0f);	//blue
	
	const glm::vec3 shift(2, 1, 3);

	Vertex vertices[] = { 
		Vertex(glm::vec3(-1.0, 1.0, 1.0), red),
		Vertex(glm::vec3(-1.0, -1.0, 1.0), red),
		Vertex(glm::vec3(1.0, -1.0, 1.0), red),
		Vertex(glm::vec3(1.0, 1.0, 1.0), red),

		Vertex(glm::vec3(-1.0, 1.0, -1.0), blue),
		Vertex(glm::vec3(-1.0, -1.0, -1.0), blue),
		Vertex(glm::vec3(1.0, -1.0, -1.0), blue),
		Vertex(glm::vec3(1.0, 1.0, -1.0), blue),
	};

	GLushort indices[] = {
		0, 2, 1, 
		0, 3, 2, 

		4, 5, 6,
		4, 6, 7, 

		3, 6, 2, 
		3, 7, 6, 

		0, 1, 5, 
		0, 5, 4, 

		0, 4, 3, 
		4, 7, 3,

		1, 2, 6, 
		1, 6, 5
	};

	//offset
	glm::vec3 offsets[] = {
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-2.0f, 1.0f, -10.0f),
		glm::vec3(3.0f, -3.0f, 0.0f),
		glm::vec3(3.0f, -1.333f, 0.0f),
	};

	Mesh mesh(	vertices, sizeof(vertices) / sizeof(vertices[0]), 
				indices, sizeof(indices) / sizeof(indices[0]),
				offsets, sizeof(offsets) / sizeof(offsets[0]));
	
	Transform transform;

	Camera camera( glm::vec3(0, 0, 10), 70.0f, (float) (WIDTH / HEIGHT), 0.01f, 1000.0f);

	while (!display.IsClosed()) {
		display.Clear(0.05, 0.66, 0.55, 1.0);

		shader.Bind();
		shader.Update(transform, camera);		

		mesh.Draw();
		display.Update(camera);
	}

	return 0;
}