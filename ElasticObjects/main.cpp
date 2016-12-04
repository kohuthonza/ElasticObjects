#include "display.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "plane.h"
#include "vertex.h"
#include "spring.h"
#include "object.h"
#include "sim.h"

#include <GL/glew.h>

#include <chrono>

#define WIDTH 1024
#define HEIGHT 768

using glm::vec3;

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

void SimTest(int milliseconds) {
	Display display(WIDTH, HEIGHT, "OpenGL Test");
	Shader shader("basic");
	Transform transform;
	Camera camera(glm::vec3(0, 0, 10), 70.0f, (float)(WIDTH / HEIGHT), 0.01f, 1000.0f);
	
	unsigned int frameCounter = 0;

	float maxPossible_dt = 0.1f;
	float dt = milliseconds / 1000.f;
	int numOfIterations = (int)(dt / maxPossible_dt) + 1;

	static const float slowMotionRatio = 50.0f;
	static float timeElapsed = 0.f;

	dt /= slowMotionRatio;

	if (numOfIterations != 0)                       // Avoid Division By Zero.
		dt = dt / numOfIterations;

	timeElapsed += dt;	

	Sim ms;

	while (!display.IsClosed()) {
		display.Clear(0.01f, 0.16f, 0.15f, 1.0f);

		shader.Bind();
		shader.Update(transform, camera);

		ms.Operate(dt, shader.GetProgram(), camera.GetPos());

		display.Update(camera, ms);

		frameCounter++;
		//UpdateFPS(frameCounter);
	}
}

#undef main
int main() {
	SimTest(20);
}