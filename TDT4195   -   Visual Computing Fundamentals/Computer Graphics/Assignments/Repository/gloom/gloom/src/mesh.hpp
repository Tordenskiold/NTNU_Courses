#pragma once

#include <string>
#include <vector>
#include <cstring>

struct float4 {
public:
	float x;
	float y;
	float z;
	float w;

	float4() = default;
	float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
};

struct float3 {
public:
	float x;
	float y;
	float z;

	float3() = default;
	float3(float x, float y, float z) : x(x), y(y), z(z) { }
};

class float2 {
public:
	float x;
	float y;

	float2() = default;
	float2(float x, float y) : x(x), y(y) { }
};

struct int2 {
public:
	int x;
	int y;

	int2() = default;
	int2(int x, int y) : x(x), y(y) { }
};

class VectorMesh {
public:
	std::string name;
	std::vector<float4> vertices;
	std::vector<float4> colours;
	std::vector<float3> normals;
	std::vector<unsigned int> indices;

	VectorMesh(std::string vname) : name(vname) {
	    // Trading memory usage for speed
	    vertices.reserve(200000);
        normals.reserve(200000);
        indices.reserve(200000);
	}

	bool hasNormals;

	unsigned long faceCount() {
		return (this->vertices.size() / 3);
	}
};

class Mesh {
public:
	std::string name;
	std::vector<float> vertices;
	std::vector<float> colours;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	Mesh(std::string vname) : name(vname) { }
	Mesh(VectorMesh &mesh) {
		name = mesh.name;
	    vertices.reserve(mesh.vertices.size() * 3);
		for (unsigned int i = 0; i < mesh.vertices.size(); i++) {
			float4 vertex = mesh.vertices.at(i);
			vertices.emplace_back(vertex.x);
			vertices.emplace_back(vertex.y);
			vertices.emplace_back(vertex.z);
		}
        normals.resize(mesh.normals.size() * 3);
        indices.resize(mesh.indices.size());
		std::memcpy(normals.data(),  mesh.normals.data(),  mesh.normals.size() * 3 * sizeof(float));
		std::memcpy(indices.data(),  mesh.indices.data(),  mesh.indices.size() * sizeof(unsigned int));
	}

	unsigned int vertexCount() {
		return (this->vertices.size()) / 3;
	}

};


