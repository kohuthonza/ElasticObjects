#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "plane.h"

#include <GL/glew.h>
#include <glm\glm.hpp>

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

void Run3DTest(){
	Display display(WIDTH, HEIGHT, "OpenGL Test");
	Shader shader("basic");

	glm::vec3 red(1.0f, 0.0f, 0.0f);	//red
	glm::vec3 blue(0.0f, 0.0f, 1.0f);	//blue

	Vertex vertices[] = {
		Vertex(glm::vec3(-1.0, 1.5, 1.0), red),//front
		Vertex(glm::vec3(-1.0, -1.0, 1.0), red),
		Vertex(glm::vec3(1.0, -1.0, 1.0), red),
		Vertex(glm::vec3(1.0, 1.0, 1.0), red),

		Vertex(glm::vec3(-1.0, 1.0, -1.0), blue),//back
		Vertex(glm::vec3(-1.0, -1.0, -1.0), blue),
		Vertex(glm::vec3(1.0, -1.0, -1.0), blue),
		Vertex(glm::vec3(1.0, 1.0, -1.0), blue),

		//Vertex(glm::vec3(1.0, 1.0, -1.0), blue), //additional front vertices
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

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]),
		indices, sizeof(indices) / sizeof(indices[0]),
		offsets, (sizeof(offsets) / sizeof(offsets[0])) / 2);

	Transform transform;

	Camera camera(glm::vec3(0, -9.5, 10), 70.0f, (float)(WIDTH / HEIGHT), 0.01f, 1000.0f);

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
}

class Object {
public:
	Object() {
		positions.push_back(glm::vec3(-0.5, -0.5, -3));
		positions.push_back(glm::vec3(0.5, -0.5, -3));
		positions.push_back(glm::vec3(0.5, 0.5, -3));
		positions.push_back(glm::vec3(-0.5, 0.5, -3));

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(1);

		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		//positions
		glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Draw() {
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
	}

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		for (int i = 0; i < positions.size(); ++i) {
			positions[i].y -= 0.005f;
		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(positions[0]), &positions[0]);

		glBindVertexArray(0);
	}

	~Object(){
		glDeleteVertexArrays(NUM_BUFFERS, &vertexArrayObject);
	}

private:
	std::vector<glm::vec3> positions;	
	std::vector<GLushort> indices;

	enum {
		POSITION_VB,
		INDICES_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
};

void Run2DSquareDeformation() {
	Display display(1024, 768, "OpenGL Test");
	Shader shader("basic");
	Transform transform;
	Camera camera(glm::vec3(0, 0, 10), 70.0f, (float)(1024 / 1024), 0.01f, 1000.0f);
	Plane plane(glm::vec3(0, 0.5, 1), glm::vec3(-5, -2, -5), 6.0f);

	Object object;

	while (!display.IsClosed()) {
		display.Clear(0.05, 0.66, 0.55, 1.0);

		shader.Bind();
		shader.Update(transform, camera);

		object.Update();
		object.Draw();

		plane.Draw();

		display.Update(camera);
	}
}

#undef main
int main(void) {
	Run2DSquareDeformation();
}