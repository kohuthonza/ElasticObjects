#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "plane.h"

#include <GL/glew.h>

#include <chrono>


#define WIDTH 800
#define HEIGHT 600

static void UpdateFPS(unsigned int &frameCounter) {
	static std::chrono::time_point<std::chrono::system_clock> start, end;

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	if (elapsed_seconds.count() >= 0.5) {
		std::cout << "FPS: " << frameCounter * 2 << std::endl;;
		frameCounter = 0;
		start = std::chrono::system_clock::now();
	}
}

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

	//offset, direction vector
	glm::vec3 offsets[] = {
		glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -0.05f, 0.0f),
		glm::vec3(-2.0f, 1.0f, -10.0f), glm::vec3(0.0f, -0.05f, 0.0f),
		glm::vec3(3.0f, -4.0f, 0.0f), glm::vec3(0.0f, -0.09f, 0.0f),
	};

	Mesh mesh(	vertices, sizeof(vertices) / sizeof(vertices[0]), 
				indices, sizeof(indices) / sizeof(indices[0]),
				offsets, (sizeof(offsets) / sizeof(offsets[0]))/2 );
	
	Transform transform;

	Camera camera( glm::vec3(0, 0, 10), 70.0f, (float) (WIDTH / HEIGHT), 0.01f, 1000.0f);

	Plane plane(glm::vec3(0, 1, 1));

	unsigned int frameCounter = 0;

	while (!display.IsClosed()) {
		display.Clear(0.05, 0.66, 0.55, 1.0);

		shader.Bind();
		shader.Update(transform, camera);		

		mesh.Draw();
		plane.Draw();

		mesh.Update(offsets, (sizeof(offsets) / sizeof(offsets[0])) / 2, plane);

		display.Update(camera);

		frameCounter++;
		UpdateFPS(frameCounter);
	}

	return 0;
}