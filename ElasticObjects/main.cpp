#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "plane.h"

#include <GL/glew.h>

#include <chrono>


#define WIDTH 1024
#define HEIGHT 768

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


static int RunBoxes(void) {
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

class Mass {
public:
	Mass(float $mass) {
		m = $mass;

		vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));
		vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));
		vertices.push_back(glm::vec3(1.0, -1.0, 1.0));
		vertices.push_back(glm::vec3(1.0, 1.0, 1.0));

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		//positions
		glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//colors
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(red), &red, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Init(){
		force = glm::vec3(0, 0, 0);
	}

	void Draw() {
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}

	void ApplyForce(glm::vec3 $force) {
		force += $force;
	}

	void Simulate(float dt) {
		vel += (force / m) * dt;		

		glm::vec3 newPosVector = vel * dt;
		pos += newPosVector;

		for (int a = 0; a < vertices.size(); ++a) {
			vertices[a] += newPosVector;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
	}

	void SetPos(glm::vec3 $pos) {
		pos = $pos;
		for (int a = 0; a < vertices.size(); ++a) {
			vertices[a] += pos;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
	}

	glm::vec3 GetPos() { return pos; }

	void SetVel(glm::vec3 $vel) {
		vel = $vel;
	}	

	float m;

private:	

	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 force;

	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);	//red

	std::vector<glm::vec3> vertices;
	std::vector<GLushort> indices;
	
	enum {
		POSITION_VB,
		COLOR_VB,
		INDICES_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];

};

class Simulation {
public:
	int numOfMasses;
	Mass **masses;

	Simulation(int $numMasses, float $m) {
		numOfMasses = $numMasses;

		masses = new Mass*[numOfMasses];

		for (int i = 0; i < numOfMasses; ++i) {
			masses[i] = new Mass($m);
		}
	}

	~Simulation() {
		release();
	}

	virtual void release()                          // Delete The Masses Created.
	{
		for (int a = 0; a < numOfMasses; ++a)                // We Will Delete All Of Them.
		{
			delete(masses[a]);
			masses[a] = NULL;
		}

		delete(masses);
		masses = NULL;
	}

	Mass* getMass(int index)
	{
		if (index < 0 || index >= numOfMasses)                // If The index Is Not In The Array.
			return NULL;                        // Then Return NULL.

		return masses[index];                       // Get The Mass At The index.
	}

	virtual void init()                         // This Method Will Call The init() Method Of Every Mass.
	{
		for (int a = 0; a < numOfMasses; ++a)                // We Will init() Every Mass.
			masses[a]->Init();                   // Call init() Method Of The Mass.
	}

	virtual void Solve() {

	}

	virtual void Simulate(float dt) {
		for (int a = 0; a < numOfMasses; ++a)                // We Will Iterate Every Mass.
			masses[a]->Simulate(dt);
	}

	virtual void Draw() {
		for (int a = 0; a < numOfMasses; ++a)
			masses[a]->Draw();
	}

	virtual void Operate(float dt) {
		init();
		Solve();
		Simulate(dt);
		Draw();
	}
};

class ConstantVelocity : public Simulation {
public:
	ConstantVelocity() : Simulation(2, 1.f) {
		masses[0]->SetPos(glm::vec3(0.f, 3.3f, 0.f));
		masses[0]->SetVel(glm::vec3(1.5f, 0.95f, 0.f));
		
		masses[1]->SetPos(glm::vec3(0.2f, 0.5f, 0.f));
		masses[1]->SetVel(glm::vec3(0.f, -2.6f, 1.67f));
	}
};

class Gravitation : public Simulation {
private:
	glm::vec3 gravitation;

public:
	Gravitation(glm::vec3 grav) : Simulation(2, 30.55f) {
		gravitation = grav;

		masses[0]->SetPos(glm::vec3(2.f, 3.3f, -5.f));
		masses[0]->SetVel(glm::vec3(1.5f, 0.95f, 0.f));

		masses[1]->SetPos(glm::vec3(2.f, 0.5f, -5.f));
		masses[1]->SetVel(glm::vec3(0.f, -2.6f, 1.67f));
	}

	virtual void Solve() {
		for (int i = 0; i < numOfMasses; ++i) {
			masses[i]->ApplyForce(gravitation * masses[i]->m);
		}
	}
};

class MassConnectedSpring : public Simulation {
public:
	float K;

	glm::vec3 connectionPos;

	MassConnectedSpring(float springConstant) : Simulation(1, 1.f) {
		K = springConstant;
		connectionPos = glm::vec3(0.f, 0.f, 0.f);
		masses[0]->SetPos(glm::vec3(0.f, -1.5f, 0.f));
		masses[0]->SetVel(glm::vec3(0.f, 0.f, 0.0f));
	}

	virtual void Solve() {
		for (int i = 0; i < numOfMasses; ++i) {
			glm::vec3 springVector = glm::vec3(masses[i]->GetPos() - connectionPos);
			masses[i]->ApplyForce(-springVector * K);
		}
	}
};


void SimTest(int milliseconds) {
	Display display(WIDTH, HEIGHT, "OpenGL Test");
	Shader shader("basic");
	Transform transform;
	Camera camera(glm::vec3(0, 0, 10), 70.0f, (float)(WIDTH / HEIGHT), 0.01f, 1000.0f);
	Plane plane(glm::vec3(0, 1, 1));
	
	unsigned int frameCounter = 0;

	float maxPossible_dt = 0.1f;
	float dt = milliseconds / 1000.f;
	int numOfIterations = (int)(dt / maxPossible_dt) + 1;

	static const float slowMotionRatio = 200.0f;
	static float timeElapsed = 0.f;

	dt /= slowMotionRatio;

	if (numOfIterations != 0)                       // Avoid Division By Zero.
		dt = dt / numOfIterations;

	timeElapsed += dt;

	ConstantVelocity cv;
	Gravitation gv(glm::vec3(0.f, -9.81f, 0.f));
	MassConnectedSpring mcs(50.f);

	while (!display.IsClosed()) {
		display.Clear(0.01, 0.16, 0.15, 1.0);

		shader.Bind();
		shader.Update(transform, camera);

		//cv.Operate(dt);
		//gv.Operate(dt);
		mcs.Operate(dt);
		plane.Draw();

		display.Update(camera);

		frameCounter++;
		UpdateFPS(frameCounter);
	}
}

#undef main
int main() {
	SimTest(20.0f);
}