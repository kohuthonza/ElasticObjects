#include "object.h"
#include "OBJ_Loader.h"
#include <iostream>
#include <random>
#include <chrono>
#include <string>

float TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	glm::vec3 VectorAB = A - B;
	glm::vec3 VectorAC = A - C;

	glm::vec3 CrossProduct = glm::cross(VectorAB, VectorAC);
	
	return 0.5 * glm::length(CrossProduct);
}

bool Object::VectorIntersectsTriangle(/* VECTOR: */ glm::vec3 Origin, glm::vec3 Dir, /* TRIANGLE: */ glm::vec3 V1, glm::vec3 V2, glm::vec3 V3) {
	/* Möller–Trumbore intersection algorithm, pseudo-code source: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm */

#define EPSILON 0.0001

	glm::vec3 e1, e2;  //Edge1, Edge2
	glm::vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	//Find vectors for two edges sharing V1
	e1 = V2 - V1;
	e2 = V3 - V1;
	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(Dir, e2);
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = glm::dot(e1, P);
	//NOT CULLING
	if (det > -EPSILON && det < EPSILON) return false;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T =  Origin - V1;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return false;

	//Prepare to test v parameter
	Q = glm::cross(T, e1);

	//Calculate V parameter and test bound
	v = glm::dot(Dir, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return false;

	t = glm::dot(e2, Q) * inv_det;

	if (t > EPSILON) { //ray intersection
		return true;
	}

	// No hit, no win
	return false;
}

bool Object::IsInside(glm::vec3 point) {
	/*
	* Returns TRUE if given point is inside of the object
	* Otherwise returns FALSE
	*/

	/*
	* lets assume there is a vector from given point in any direction; count number of triagles whose body is intersected by the vector
	* From the number of intersected triangles we can easily determine if the point is inside or not.
	*/

	glm::vec3 Direction(1.0, 1.0, 1.0);
	int intersects = 0;

	for (unsigned int i = 0; i < indices.size(); i += 3) {
		unsigned int vert_index[3];

		vert_index[0] = indices[i];
		vert_index[1] = indices[i + 1];
		vert_index[2] = indices[i + 2];
		
		if (VectorIntersectsTriangle(point, Direction, /* TRIANGLE: */ vertices[vert_index[0]], vertices[vert_index[1]], vertices[vert_index[2]]))
			intersects++;
	}
	//std::cout << "Number of triangles hit: " << intersects << std::endl << std::flush;
	
	if (intersects % 2 == 0) {
		// out
		//std::cout << "Intesects == FALSE" << std::endl << std::flush;
		return false;
	}
	else {
		// in
		//std::cout << "Intesects == TRUE" << std::endl << std::flush;
		return true;
	}
}

void Object::CalculateBodyVolume() {
	
	assert(springs.size() > 0 && "Volume calculation can be done only alfter an object is fully initialized.");
	assert(vertices.size() > 0 && "Volume calculation can be done only alfter an object is fully initialized.");
	assert(indices.size() > 0 && "Volume calculation can be done only alfter an object is fully initialized.");
	assert(verts.size() > 0 && "Volume calculation can be done only alfter an object is fully initialized.");
	
	std::cout << "Calculating body volume... ";
	
	/*
	* Calculate body volume using Monte Carlo method 
	*
	*/

	// we have a bounding-box of the object, entire object is expected to be in the bounding-box
	// lets
	AABB *BBCoords = GetAABB();
		
	float BoundingBoxSizeX = fabs(BBCoords->max.x - BBCoords->min.x);
	float BoundingBoxSizeY = fabs(BBCoords->max.y - BBCoords->min.y);
	float BoundingBoxSizeZ = fabs(BBCoords->max.z - BBCoords->min.z);
	
	float BoundingBoxVolume =  BoundingBoxSizeX * BoundingBoxSizeY * BoundingBoxSizeZ;

	if (indices.size() > 500.0) {
		BodyVolume = BoundingBoxVolume;
		std::cout << " Done. (Body volume: " << BodyVolume << ")" << std::endl;
		return;
	}
	//std::cout << "BBox volume: " << BoundingBoxVolume << std::endl;

	//std::cout << "BBox size x: " << fabs(BBCoords->max.x - BBCoords->min.x) << std::endl;
	//std::cout << "BBox size Y: " << fabs(BBCoords->max.y - BBCoords->min.y) << std::endl;
	//td::cout << "BBox size z: " << fabs(BBCoords->max.z - BBCoords->min.z) << std::endl;

	const int NumberIterations = 1000000; 
	int hit = 0;

	for (int i = 0; i < NumberIterations; i++) {
		float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		
		// generate random point inside of the bounding box
		float xCoord = BBCoords->min.x + rx * BoundingBoxSizeX;
		float yCoord = BBCoords->min.y + ry * BoundingBoxSizeY;
		float zCoord = BBCoords->min.z + rz * BoundingBoxSizeZ;
		
		// is the point inside or outside of the objects?
		if (IsInside(glm::vec3(xCoord, yCoord, zCoord))) {
			hit++;
		}
	}
	BodyVolume = BoundingBoxVolume * ((float)hit / (float)NumberIterations);

	std::cout << " Done. (Body volume: " << BodyVolume << ")" << std::endl;
}



Object::Object(glm::vec3 $normal, glm::vec3 point1, glm::vec3 point2, float D){
	isPlane = true;
	normal = $normal;
	pointOnPlane = point1;

	glm::vec3 tangent = glm::normalize(point2 - point1);
	glm::vec3 bitangent = glm::cross(tangent, normal);	

	vertices.push_back(point1 - tangent*D - bitangent*D);
	vertices.push_back(point1 + tangent*D - bitangent*D);
	vertices.push_back(point1 + tangent*D + bitangent*D);
	vertices.push_back(point1 - tangent*D + bitangent*D);

	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<float> unif(0, 1);

	vec3 color( unif(rng), unif(rng), unif(rng));

	verts.push_back(new Vertex(vertices[0], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[1], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[2], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vertices[3], vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// positions
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDICES_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


inline void Object::AddVertex(Vertex * vertex) {
	verts.push_back(vertex);
}

inline void Object::AddIndex(GLushort index) {
	indices.push_back(index);
}

inline void Object::AddSpring(int point1, int point2, float springConstat) {
	springs.push_back(new Spring(point1, point2, verts[point1], verts[point2], glm::distance(verts[point2]->pos, verts[point1]->pos), springConstat));
}

bool Object::SpringExists(int point1, int point2) {
	for (unsigned int i = 0; i < springs.size(); ++i) {
		if (springs[i]->Index1() == point1 && springs[i]->Index2() == point2)
			return true;
		if (springs[i]->Index1() == point2 && springs[i]->Index2() == point1)
			return true;
	}

	return false;
}

void Object::GenerateSprings_NeighboursOnly(const float Force) {
	std::cout << "Generating springs...";

	assert(vertices.size() > 0 && "Spring generation can only be done with initialized buffers.");
	assert(indices.size() > 0 && "Spring generation can only be done with initialized buffers.");
	assert(verts.size() > 0 && "Spring generation can only be done with initialized buffers.");

	for (unsigned int i = 0; i < indices.size(); i += 3) { // loop over all faces to add springs. Each edge is represented by one spring.
		unsigned int vert_index[3];

		vert_index[0] = indices[i];
		vert_index[1] = indices[i + 1];
		vert_index[2] = indices[i + 2];

		// 0 1 spring
		if (!SpringExists(vert_index[0], vert_index[1])) {
			AddSpring(vert_index[0], vert_index[1], 100.2f);
		}

		// 1 2 spring
		if (!SpringExists(vert_index[1], vert_index[2])) {
			AddSpring(vert_index[1], vert_index[2], 100.2f);
		}

		// 2 0 spring
		if (!SpringExists(vert_index[2], vert_index[0])) {
			AddSpring(vert_index[2], vert_index[0], 100.2f);
		}
	}

	std::cout << "  Springs generated. Number of springs is " << springs.size() << ". " << std::endl;
}


void Object::GenerateSprings(const float Force) {

	std::cout << "Generating springs..." ;

	assert(vertices.size() > 0 && "Spring generation can only be done with initialized buffers.");
	assert(indices.size() > 0 && "Spring generation can only be done with initialized buffers.");
	assert(verts.size() > 0 && "Spring generation can only be done with initialized buffers.");

	for (unsigned int i = 0; i < verts.size(); ++i) {
		for (unsigned int j = 0; j < verts.size(); ++j) {
			//if (!SpringExists(i, j)) {
				AddSpring(i, j, Force);
			//}

		}
	}

	std::cout << "  Springs generated. Number of springs is " << springs.size() << ". " << std::endl;
}


void Object::InitOBJTest(std::string FilePath, const float Mass,glm::vec3 offset = glm::vec3(0, 0, 0), glm::vec3 initialVel = glm::vec3(0, 0, 0)) {

	OBJ_Loader obj(FilePath);
	vertices = obj.getVertices();
	indices = obj.getIndices();
	normals = obj.getNormals();

	std::cout << "Normals size: " << normals.size() << std::endl;
	std::cout << "Indices size: " << indices.size() << std::endl;
	std::cout << "Vertices size: " << vertices.size() << std::endl;

	if (indices.size() != normals.size() * 3) { // fix normals
		normals.resize(indices.size() / 3);

		for (unsigned int i = 0; i < indices.size(); i += 3) { // loop over all faces

			glm::vec3 v[3];

			v[0] = vertices[indices[i]];
			v[1] = vertices[indices[i + 1]];
			v[2] = vertices[indices[i + 2]];

			normals[i / 3] = CalculateSurfaceNormal(v[0], v[1], v[2]);
		}
	}

	for (auto &i : vertices) {
		verts.push_back(new Vertex( i + offset, vec3(0.0, 0.0, 0.0) + initialVel, vec3(0.0, 0.0, 0.0), Mass ) );
	}
	
	GenerateBoundingBox();

	const float SpringForce = 40.0;
	GenerateSprings(SpringForce);


	CalculateBodyVolume();
	
 	for (unsigned int i = 0; i < obj.getVerticesNumber(); i++) {
		colors.push_back(red);
	}
	
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



void Object::InitTest() {

	verts.push_back(new Vertex(vec3(-1.0, -1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, -1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 1.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));


	vertices.push_back(verts[0]->pos);
	vertices.push_back(verts[1]->pos);
	vertices.push_back(verts[2]->pos);
	vertices.push_back(verts[3]->pos);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	AddSpring(0, 1, 1000.0f);
	AddSpring(1, 2, 1000.0f);
	AddSpring(3, 2, 1000.0f);
	AddSpring(0, 3, 1000.0f);
	AddSpring(0, 2, 1000.0f);
	AddSpring(1, 3, 1000.0f);

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

void Object::InitTestHexa() {
	verts.push_back(new Vertex(vec3(-0.25, 3.0, -1.0), vec3(0.4, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 2.0f));
	verts.push_back(new Vertex(vec3(0.25, 3.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 3.75, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(1.0, 4.25, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(0.25, 5.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-0.25, 5.0, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 4.25, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));
	verts.push_back(new Vertex(vec3(-1.0, 3.75, -1.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 1.0f));

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

	AddSpring(0, 6, 100.0f);
	AddSpring(0, 5, 100.0f);
	AddSpring(0, 4, 100.0f);
	AddSpring(0, 3, 100.0f);

	AddSpring(1, 7, 100.0f);
	AddSpring(1, 6, 100.0f);
	AddSpring(1, 5, 100.0f);
	AddSpring(1, 4, 100.0f);

	AddSpring(2, 7, 1.0f);
	AddSpring(2, 6, 1.0f);
	AddSpring(2, 5, 1.0f);
	AddSpring(2, 0, 1.0f);

	AddSpring(0, 1, 1.0f);
	AddSpring(1, 2, 1.0f);
	AddSpring(2, 3, 1.0f);
	AddSpring(3, 4, 1.0f);
	AddSpring(4, 5, 1.0f);
	AddSpring(5, 6, 1.0f);
	AddSpring(6, 7, 1.0f);
	AddSpring(7, 0, 1.0f);

	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);
	colors.push_back(green);

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

void Object::Solve() {
	if (isPlane)
		return;

	//std::cout << "spring solve" << std::endl << std::flush;
	for (unsigned int i = 0; i < springs.size(); ++i) {
		springs[i]->Solve();
	}
}

void Object::Simulate(float dt) {
	if (isPlane)
		return;

	for (unsigned int i = 0; i < verts.size(); ++i) {
		/* For more info follow http://academypublisher.com/jcp/vol02/no08/jcp02083443.pdf */

		/*
		* Gravitational force
		*/
		verts[i]->ApplyForce(gravitation * verts[i]->mass);

		/*
		* Damping force
		*/
		verts[i]->ApplyForce(-verts[i]->vel * airFrictionConstant);

		/*
		* Spring forces
		*/
		// computed in the Solve() method


		/*
		*  Inside pressure (Simulate ideal gas pressure from the inside of the object)
		*/
		//Calculate the pressure value
		//const float Na = 6.02214e23; //- Avogardo number
		//const float kb = 1.380648e-23; //- Bolzman konstant
		//float R = Na * kb;
		float R = 8.3144598; //- ideal gas constant (https://en.wikipedia.org/wiki/Gas_constant)
		float Temperature = 1; //- Temperature in Kelvin
					
		glm::vec3 InsideForceSum(0.0, 0.0, 0.0);
		
		for (unsigned int i = 0; i < indices.size(); i+=3) { // loop over all faces
			glm::vec3 v[3];
		
			v[0] = vertices[indices[ i ]];
			v[1] = vertices[indices[ i + 1 ]];
			v[2] = vertices[indices[ i + 2 ]];

			/*
			* Check if face contains current vertice, continue if not
			*/
			bool contains = false;
			contains |= (indices[i] == i);
			contains |= (indices[i + 1] == i);
			contains |= (indices[i + 2] == i);
			if (!contains) continue;

			
			float FaceArea = TriangleArea(v[0], v[1], v[2]);

			// Accumulate force over all neigbouring faces
			InsideForceSum += FaceArea * normals[i / 3] * (1 / BodyVolume) * R * Temperature;
			//std::cout << "FaceArea = " << FaceArea << std::endl;
		}
		verts[i]->ApplyForce(InsideForceSum);
				
	}


	for (unsigned int i = 0; i < verts.size(); ++i) {
		verts[i]->vel += (verts[i]->force / verts[i]->mass) * dt;
		vec3 newPosVector = verts[i]->vel * dt;
		verts[i]->pos += newPosVector;

		vertices[i] = verts[i]->pos;

		verts[i]->force = vec3(0, 0, 0);
	}

	/*
	* With new position update normal of each face
	*/
	for (unsigned int i = 0; i < indices.size(); i += 3) { // loop over all faces
		
		glm::vec3 v[3];

		v[0] = vertices[indices[i]];
		v[1] = vertices[indices[i + 1]];
		v[2] = vertices[indices[i + 2]];

		normals[i/3] = CalculateSurfaceNormal(v[0], v[1], v[2]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
}

glm::vec3 Object::CalculateSurfaceNormal(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3) {
	/* Pseudo-code source: https://www.opengl.org/wiki/Calculating_a_Surface_Normal */
	
	glm::vec3 U = P2 - P1;
	glm::vec3 V = P3 - P1;

	glm::vec3 Normal;

	Normal.x = (U.y * V.z) - (U.z * V.y);
	Normal.y = (U.z * V.x) - (U.x * V.z);
	Normal.z = (U.x * V.y) - (U.y * V.x);

	return Normal;
}

void Object::Draw() {
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Object::GenerateBoundingBox(){
	vec3 tempMin = vertices[0];
	vec3 tempMax = vertices[0];

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		if (vertices[i].x < tempMin.x) tempMin.x = vertices[i].x;
		if (vertices[i].y < tempMin.y) tempMin.y = vertices[i].y;
		if (vertices[i].z < tempMin.z) tempMin.z = vertices[i].z;

		if (vertices[i].x > tempMax.x) tempMax.x = vertices[i].x;
		if (vertices[i].y > tempMax.y) tempMax.y = vertices[i].y;
		if (vertices[i].z > tempMax.z) tempMax.z = vertices[i].z;
	}
	
	aabbCoords.max = tempMax;
	aabbCoords.min = tempMin;
}

static vec3 projectUonV(vec3 u, vec3 v) {
	return ( v * (glm::dot(u, v)/glm::dot(v, v)) );
}

void Object::ResolveVertices(Object * other) {
	
	if (isPlane || other->isPlane) {
		Object *planeObject;
		Object *generalObject;
		if (isPlane) {
			planeObject = this;
			generalObject = other;
		}
		else {
			planeObject = other;
			generalObject = this;
		}
		

		for (unsigned int i = 0; i < generalObject->GetVerts().size(); ++i) {
			float distance = glm::dot((generalObject->GetVerts()[i]->pos - planeObject->pointOnPlane), planeObject->normal);

			if (abs(distance) < 0.5f) {				
				vec3 rv = -generalObject->GetVerts()[i]->vel;
				float velAlongNormal = glm::dot(rv, planeObject->normal);
				float e = 0.1f;

				float j = -(1.0f - e) * velAlongNormal;
				j /= 1 / generalObject->GetVerts()[i]->mass;

				vec3 impulse = j * planeObject->normal;
				generalObject->GetVerts()[i]->vel -= (1.0f / generalObject->GetVerts()[i]->mass) * impulse;
			}
		}		
	}
	else {
		for (unsigned int i = 0; i < verts.size(); ++i) {
			for (unsigned int j = 0; j < other->GetVerts().size(); ++j) {
				float dist = glm::distance(verts[i]->pos, other->GetVerts()[j]->pos);
				if (dist <= 0.5f) {
					auto ObjectA = verts[i];
					auto ObjectB = other->GetVerts()[j];

					vec3 nv1, nv2;

					nv1 = ObjectA->vel;
					nv1 += projectUonV(ObjectB->vel, ObjectB->pos - ObjectA->pos);
					nv1 -= projectUonV(ObjectA->vel, ObjectA->pos - ObjectB->pos);

					nv2 = ObjectB->vel;
					nv2 += projectUonV(ObjectA->vel, ObjectB->pos - ObjectA->pos);
					nv2 -= projectUonV(ObjectB->vel, ObjectA->pos - ObjectB->pos);

					ObjectA->vel = nv1;
					ObjectB->vel = nv2;

					std::cout << "vertex " << i << " and " << j << " are in col" << std::endl;
				}
			}
		}
	}
}

