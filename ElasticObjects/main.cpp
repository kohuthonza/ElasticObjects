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

	Plane plane(glm::vec3(0, 1, 1), -0.5f);

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

struct MyVertex
{
	MyVertex(vec3 $pos, vec3 $vel, vec3 $force, float $mass) {
		pos = $pos;
		vel = $vel;
		force = $force;
		mass = $mass;
	}

	vec3 pos;
	vec3 vel;
	vec3 force;
	float mass;

	void ApplyForce(vec3 $force) {
		force += $force;
	}
};

class MySpring {
private:
	MyVertex *point1;
	MyVertex *point2;

	float springConstant;
	float springLength;

	const float airFrictionConstant = 0.2f;

public:
	MySpring(MyVertex *$point1, MyVertex *$point2, float springLen, float springConst) {
		point1 = $point1;
		point2 = $point2;

		springLength = springLen;
		springConstant = springConst;
	}

	void Solve() {
		glm::vec3 springVector = point1->pos - point2->pos;

		float r = glm::length(springVector);

		glm::vec3 force(0, 0, 0);

		if (r != 0.0f)
			force += -(springVector / r) * (r - springLength) * springConstant;

		force += -(point1->vel - point2->vel)*airFrictionConstant;
		point1->ApplyForce(force);
		point2->ApplyForce(-force);
	}
};


class Object {
public:
	Object() {};

	void AddVertex(MyVertex *vertex) {
		verts.push_back(vertex);
	}

	void AddIndex(GLushort index) {
		indices.push_back(index);
	}

	void AddSpring(int point1, int point2, float springConstat) {
		springs.push_back(new MySpring(verts[point1], verts[point2], glm::distance(verts[point2]->pos, verts[point1]->pos), springConstat));
	}

	void InitTest() {
		verts.push_back(new MyVertex(vec3(-1.0, -1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(-10.0, 0.0, 0.0), 10.0f));
		verts.push_back(new MyVertex(vec3(1.0, -1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 10.0f));
		verts.push_back(new MyVertex(vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(-0.099, 0.0, 0.0), 10.0f));
		verts.push_back(new MyVertex(vec3(-1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, -400.0, 0.0), 10.0f));

		vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));
		vertices.push_back(glm::vec3(1.0, -1.0, 1.0));
		vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
		vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

		AddSpring(0, 1, 1.0f);
		AddSpring(1, 2, 1.0f);
		AddSpring(3, 2, 1.0f);
		AddSpring(0, 3, 1.0f);
		AddSpring(0, 2, 10.0f);
		AddSpring(1, 3, 1.0f);

		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);

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
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors), &colors[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void InitTestHexa() {
		verts.push_back(new MyVertex(vec3(-0.25, -1.0, 1.0), vec3(0.4, 0.4, 0.0), vec3(0.0, 0.0, 0.0), 2.0f));
		verts.push_back(new MyVertex(vec3(0.25, -1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(1.0, -0.25, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(1.0, 0.25, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(0.25, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(-0.25, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(-1.0, 0.25, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
		verts.push_back(new MyVertex(vec3(-1.0, -0.25, 1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 10.0f));

		vertices.push_back(verts[0]->pos);
		vertices.push_back(verts[1]->pos);
		vertices.push_back(verts[2]->pos);
		vertices.push_back(verts[3]->pos);
		vertices.push_back(verts[4]->pos);
		vertices.push_back(verts[5]->pos);
		vertices.push_back(verts[6]->pos);
		vertices.push_back(verts[7]->pos);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(1);

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(4);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(5);
		indices.push_back(4);

		indices.push_back(0);
		indices.push_back(6);
		indices.push_back(5);

		indices.push_back(0);
		indices.push_back(7);
		indices.push_back(6);

		AddSpring(0, 6, 1.0f);
		AddSpring(0, 5, 1.0f);
		AddSpring(0, 4, 1.0f);
		AddSpring(0, 3, 1.0f);

		AddSpring(1, 7, 1.0f);
		AddSpring(1, 6, 1.0f);
		AddSpring(1, 5, 1.0f);
		AddSpring(1, 4, 1.0f);

		AddSpring(2, 7, 1.0f);
		AddSpring(2, 6, 1.0f);
		AddSpring(2, 5, 1.0f);
		AddSpring(2, 0, 1.0f);

		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);
		colors.push_back(red);

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
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors), &colors[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Solve() {
		for (int i = 0; i < springs.size(); ++i) {
			springs[i]->Solve();
		}
	}

	void Simulate(float dt) {
		for (int i = 0; i < verts.size(); ++i) {
			//verts[i]->ApplyForce(gravitation * verts[i]->mass);
			verts[i]->ApplyForce(-verts[i]->vel * airFrictionConstant);

			verts[i]->vel += (verts[i]->force / verts[i]->mass) * dt;
			vec3 newPosVector = verts[i]->vel * dt;
			verts[i]->pos += newPosVector;

			vertices[i] += newPosVector;

			verts[i]->force = vec3(0, 0, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
	}

	void Draw() {
		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}

private:
	const vec3 gravitation = vec3(0, -9.81f, 0);
	const float airFrictionConstant = 0.2f;

	std::vector<MyVertex *> verts;
	std::vector<MySpring *> springs;

	std::vector<glm::vec3> vertices;
	std::vector<GLushort> indices;
	std::vector<glm::vec3> colors;	

	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);	//red

	enum {
		POSITION_VB,
		COLOR_VB,
		INDICES_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
};

class MySim {
public:
	MySim() {
		objects.push_back(new Object());
		objects.push_back(new Object());
		objects[0]->InitTestHexa();
		objects[1]->InitTest();
	}

	virtual void init()                         // This Method Will Call The init() Method Of Every Mass.
	{
		
	}

	virtual void Solve() {
		for (int a = 0; a < objects.size(); ++a)               
			objects[a]->Solve();
	}

	virtual void Simulate(float dt) {
		for (int a = 0; a < objects.size(); ++a)                // We Will Iterate Every Mass.
			objects[a]->Simulate(dt);
	}

	virtual void Draw() {
		for (int a = 0; a < objects.size(); ++a)
			objects[a]->Draw();
	}

	virtual void Operate(float dt) {
		init();
		Solve();
		Simulate(dt);
		Draw();
	}

private:
	std::vector<Object *> objects;

};

/*
class SquareSimulation {
	void Solve() {
		for (int a = 0; a < 6; ++a) {
			springs[a]->Solve();
		}

		for (int a = 0; a < numOfMasses; ++a) {
			masses[a]->ApplyForce(gravitation * masses[a]->m);
			masses[a]->ApplyForce(-masses[a]->GetVel() * airFrictionConstant);

			if (masses[a]->GetPos().y < groundHeight) {
				vec3 v;
				v = masses[a]->GetVel();

				v.y = 0;

				masses[a]->ApplyForce(-v * groundFrictionConstant);

				v = masses[a]->GetVel();
				v.x = 0;
				v.z = 0;

				if (v.y < 0)
					masses[a]->ApplyForce(-v*groundAbsorptionConstant);

				vec3 force = vec3(0, groundRepulsionConstant, 0) * (groundHeight - masses[a]->GetPos().y);

				masses[a]->ApplyForce(force);
			}
		}
	}
};
*/

void SimTest(int milliseconds) {
	Display display(WIDTH, HEIGHT, "OpenGL Test");
	Shader shader("basic");
	Transform transform;
	Camera camera(glm::vec3(0, 0, 10), 70.0f, (float)(WIDTH / HEIGHT), 0.01f, 1000.0f);
	Plane plane(glm::vec3(0, 1, 1), -3.5f);
	
	unsigned int frameCounter = 0;

	float maxPossible_dt = 0.1f;
	float dt = milliseconds / 1000.f;
	int numOfIterations = (int)(dt / maxPossible_dt) + 1;

	static const float slowMotionRatio = 5.0f;
	static float timeElapsed = 0.f;

	dt /= slowMotionRatio;

	if (numOfIterations != 0)                       // Avoid Division By Zero.
		dt = dt / numOfIterations;

	timeElapsed += dt;	

	MySim ms;

	while (!display.IsClosed()) {
		display.Clear(0.01, 0.16, 0.15, 1.0);

		shader.Bind();
		shader.Update(transform, camera);

		ms.Operate(dt);

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