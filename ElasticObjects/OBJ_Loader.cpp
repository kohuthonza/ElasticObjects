#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "OBJ_Loader.h"

using namespace std;
/*
Brief .OBJ format description:

file example:
	"# Blender3D v249 OBJ File: untitled.blend
	# www.blender3d.org
	mtllib cube.mtl
	v 1.000000 -1.000000 -1.000000
	...
	v -1.000000 1.000000 -1.000000
	vt 0.748573 0.750412
	...
	vt 0.498993 0.250415
	vn 0.000000 0.000000 -1.000000
	..
	vn -0.000000 -1.000000 0.000000
	usemtl Material_ray.png
	s off
	f 5/1/1 1/2/1 4/3/1
	...
	f 5/1/1 4/3/1 8/4/1"

# is a comment
v is a vertex
vt is the texture coordinate of one vertex
vn is the normal of one vertex
f is a face:
	- consists of three vertices (5/1/1 1/2/1 4/3/1)
	- first number == number of vertex
	- second number == number of texture coordinate
	- third number == normal vector

*/

OBJ_Loader::OBJ_Loader(const string filename)
{
	ifstream inputFile(filename, ios::in);
	if (inputFile.fail()) {
		cerr << "Failed opening " << filename << endl;
		exit(-1);
	}

	string l;
	
	while (getline(inputFile, l)) {
		if (l.substr(0, 2) == "v ") // vertex
		{
			istringstream s(l.substr(2));
			glm::vec3 vertex;
			
			s >> vertex.x;
			s >> vertex.y;
			s >> vertex.z;

			verticesNumber++;
			vertices.push_back( vertex );
		}
		else if (l.substr(0, 2) == "vn") //normals
		{
			istringstream s(l.substr(2));
			glm::vec3 normal;

			s >> normal.x;
			s >> normal.y;
			s >> normal.z;

			normals.push_back( normal );
		}
		else if (l.substr(0, 2) == "f ") //face
		{
			string s(l.substr(2));
			int vertex[3], texture[3], normal[3];
			// parse face string
			parseFace(s, vertex, texture, normal);

			//pushback values
			indices.push_back( (GLushort) (vertex[0]) );
			indices.push_back( (GLushort) (vertex[1]) );
			indices.push_back( (GLushort) (vertex[2]) );

			normalsIndices.push_back((GLushort)(normal[0]));
			normalsIndices.push_back((GLushort)(normal[1]));
			normalsIndices.push_back((GLushort)(normal[2]));
		}
		else if (l[0] == '#')
		{
			/* comment -> ignore */
		}
		else
		{
			/* anything else -> ignore */
		}
	}
	
	inputFile.close();
}

void OBJ_Loader::parseFace(std::string s, int *vertex, int *texture, int *normal) {
	string vertex_string[3] = { "0", "0", "0" };
	string texture_string[3] = { "0", "0", "0" };
	string normal_string[3] = { "0", "0", "0" };

	enum states { start0, start1, start2, v0, v1, v2, t0, t1, t2, n0, n1, n2, end_success };
	states state = start0;
	int i = 0;
	while (s[i]) { // FSM parsing string
		switch (state) {
		case start0:
			if (s[i] >= '0' && s[i] <= '9') {
				state = v0;
				i--;
			}
			else if (s[i] == ' ') {
				state = start0;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case v0:
			if (s[i] >= '0' && s[i] <= '9') {
				vertex_string[0] += s[i];
			}
			else if (s[i] == '/') {
				state = t0;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case t0:
			if (s[i] >= '0' && s[i] <= '9') {
				texture_string[0] += s[i];
			}
			else if (s[i] == '/') {
				state = n0;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case n0:
			if (s[i] >= '0' && s[i] <= '9') {
				normal_string[0] += s[i];
			}
			else if (s[i] == ' ') {
				state = start1;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case start1:
			if (s[i] >= '0' && s[i] <= '9') {
				state = v1;
				i--;
			}
			else if (s[i] == ' ') {
				state = start1;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case v1:
			if (s[i] >= '0' && s[i] <= '9') {
				vertex_string[1] += s[i];
			}
			else if (s[i] == '/') {
				state = t1;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case t1:
			if (s[i] >= '0' && s[i] <= '9') {
				texture_string[1] += s[i];
			}
			else if (s[i] == '/') {
				state = n1;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case n1:
			if (s[i] >= '0' && s[i] <= '9') {
				normal_string[1] += s[i];
			}
			else if (s[i] == ' ') {
				state = start2;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case start2:
			if (s[i] >= '0' && s[i] <= '9') {
				state = v2;
				i--;
			}
			else if (s[i] == ' ') {
				state = start2;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case v2:
			if (s[i] >= '0' && s[i] <= '9') {
				vertex_string[2] += s[i];
			}
			else if (s[i] == '/') {
				state = t2;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case t2:
			if (s[i] >= '0' && s[i] <= '9') {
				texture_string[2] += s[i];
			}
			else if (s[i] == '/') {
				state = n2;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case n2:
			if (s[i] >= '0' && s[i] <= '9') {
				normal_string[2] += s[i];
			}
			else if (s[i] == ' ') {
				state = end_success;
			}
			else {
				cerr << "Wrong obj file." << endl;
				exit(-1);
			}
			break;
		case end_success:
			break;
		}
		i++;
	}

	vertex[0] = stoi(vertex_string[0], NULL, 10) - 1;
	vertex[1] = stoi(vertex_string[1], NULL, 10) - 1;
	vertex[2] = stoi(vertex_string[2], NULL, 10) - 1;

	texture[0] = stoi(texture_string[0], NULL, 10)/* ??? */ - 1 /* ??? */;
	texture[1] = stoi(texture_string[1], NULL, 10)/* ??? */ - 1 /* ??? */;
	texture[2] = stoi(texture_string[2], NULL, 10)/* ??? */ - 1 /* ??? */;

	normal[0] = stoi(normal_string[0], NULL, 10);
	normal[1] = stoi(normal_string[1], NULL, 10);
	normal[2] = stoi(normal_string[2], NULL, 10);
}

std::vector<glm::vec3> OBJ_Loader::getVertices() {
	return vertices;
}

std::vector<GLushort> OBJ_Loader::getIndices() {
	return indices;
}

std::vector<GLushort> OBJ_Loader::getNormalsIndices() {
	return normalsIndices;
}

std::vector<glm::vec3> OBJ_Loader::getNormals() {
	return normals;
}

unsigned int OBJ_Loader::getVerticesNumber() {
	return verticesNumber;
}
OBJ_Loader::~OBJ_Loader()
{
}
