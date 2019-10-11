#include "OBJLoader.hpp"
#include <algorithm>
#include <exception>
#include "sceneGraph.hpp"
#include "toolbox.hpp"

void split(std::string &target, const char delimiter, std::vector<std::string> &res, unsigned int* outLength)
{
    size_t pos = 0;
    unsigned int count = 0;
	while ((pos = target.find(delimiter)) != std::string::npos) {
		res[count] = target.substr(0, pos);
		target.erase(0, pos + 1);
		count++;
	}
	res[count] = target;
	count++;
	*outLength = count;
}

std::vector<VectorMesh> loadWavefront(std::string const srcFile, bool quiet)
{
	std::vector<VectorMesh> meshes;
	std::ifstream objFile(srcFile);
	std::vector<float4> vertices;
	std::vector<float3> normals;

	// Defined here for performance reasons
    unsigned int parts_main_length;
	std::vector<std::string> parts_main;
    parts_main.resize(64);

    unsigned int parts_1_length;
    std::vector<std::string> parts_1;
    parts_1.resize(64);

    unsigned int parts_2_length;
    std::vector<std::string> parts_2;
    parts_2.resize(64);

    unsigned int parts_3_length;
    std::vector<std::string> parts_3;
    parts_3.resize(64);

    unsigned int parts_4_length;
    std::vector<std::string> parts_4;
    parts_4.resize(64);

	if (objFile.is_open()) {
		std::string line;
		while (std::getline(objFile, line)) {

			split(line, ' ', parts_main, &parts_main_length);

			if (parts_main_length > 0) {
				// New VectorMesh object
				if (parts_main.at(0) == "o" && parts_main_length >= 2) {
					meshes.emplace_back(parts_main[1]);
				} else if (parts_main[0] == "v" && parts_main_length >= 4) {
					vertices.emplace_back(
						std::stof(parts_main[1]),
						std::stof(parts_main[2]),
						std::stof(parts_main[3]),
						(parts_main_length >= 5) ? std::stof(parts_main[4]) : 1.0f
					);
				} else if (parts_main[0] == "vn" && parts_main_length >= 4) {
				   normals.emplace_back(
					   std::stof(parts_main[1]),
					   std::stof(parts_main[2]),
					   std::stof(parts_main[3])
				   );
			   } else if (parts_main[0] == "f" && parts_main_length >= 4) {
				   if (meshes.size() == 0) {
					   if (!quiet) {
						   	std::cout << "[WARNING] face definition found, but no object" << std::endl;
							std::cout << "[WARNING] creating object 'noname'" << std::endl;
					   }
					   meshes.emplace_back("noname");
					   //continue;
				   }

				   	VectorMesh &VectorMesh = meshes.back();

					bool quadruple = parts_main_length >= 5;

					split(parts_main[1],'/', parts_1, &parts_1_length);
					split(parts_main[2],'/', parts_2, &parts_2_length);
					split(parts_main[3],'/', parts_3, &parts_3_length);

					if (quadruple) {
						split(parts_main[4],'/', parts_4, &parts_4_length);
					}

					if (parts_1_length < 1 || parts_1_length != parts_2_length || parts_2_length != parts_3_length || (quadruple && parts_4_length != parts_1_length)) {
						if (!quiet)
							std::cout << "[WARNING] invalid face defintion '" << line << "'" << std::endl;
						continue;
					}

					VectorMesh.hasNormals = parts_1_length >= 3;

					size_t n1_index, n2_index, n3_index, n4_index;
					size_t v4_index;
					size_t v1_index = std::stoi(parts_1[0]) - 1;
					size_t v2_index = std::stoi(parts_2[0]) - 1;
					size_t v3_index = std::stoi(parts_3[0]) - 1;

					if (quadruple) {
						v4_index = std::stoi(parts_4[0]) - 1;
					}

					if (v1_index >= vertices.size() ||
						v2_index >= vertices.size() ||
						v3_index >= vertices.size() ||
						(quadruple && v4_index >= vertices.size())) {
								if (!quiet) {
									std::cout << "[WARNING] VectorMesh " << VectorMesh.name << " faces vertices(" << v1_index << ", " << v2_index << ", " << v3_index;
									if (quadruple)
										std::cout << ", " << v4_index;
									std::cout << ") do not exist!" << std::endl;
								}
								continue;
					}


					if (VectorMesh.hasNormals) {
						n1_index = std::stoi(parts_1[2]) - 1;
						n2_index = std::stoi(parts_2[2]) - 1;
						n3_index = std::stoi(parts_3[2]) - 1;
						if (quadruple) {
							n4_index = std::stoi(parts_4[2]) - 1;
						}
						if (n1_index >= normals.size() ||
							n2_index >= normals.size() ||
							n3_index >= normals.size() ||
							(quadruple && n4_index >= normals.size())) {
									if (!quiet) {
										std::cout << "[WARNING] VectorMesh " << VectorMesh.name << " faces normals(" << n1_index << ", " << n2_index << ", " << n3_index;
										if (quadruple)
											std::cout << ", " << n4_index;
										std::cout << ") do not exist!" << std::endl;
									}
									continue;
						}
					}

					if (quadruple) {
						VectorMesh.vertices.push_back(vertices[v1_index]);
						VectorMesh.vertices.push_back(vertices[v3_index]);
						VectorMesh.vertices.push_back(vertices[v4_index]);

						if (VectorMesh.hasNormals) {
							VectorMesh.normals.push_back(normals[n1_index]);
							VectorMesh.normals.push_back(normals[n3_index]);
							VectorMesh.normals.push_back(normals[n4_index]);
						} else {
							VectorMesh.normals.insert(VectorMesh.normals.end(), { 0.0f, 0.0f, 0.0f });
						}

						VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
						VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
						VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
					}

					VectorMesh.vertices.push_back(vertices[v1_index]);
					VectorMesh.vertices.push_back(vertices[v2_index]);
					VectorMesh.vertices.push_back(vertices[v3_index]);
					if (VectorMesh.hasNormals){
						VectorMesh.normals.push_back(normals[n1_index]);
						VectorMesh.normals.push_back(normals[n2_index]);
						VectorMesh.normals.push_back(normals[n3_index]);
					} else {
						VectorMesh.normals.insert(VectorMesh.normals.end(), { 0.0f, 0.0f, 0.0f });
					}

					VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
					VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
					VectorMesh.indices.push_back(unsigned(VectorMesh.indices.size()));
				}
			}
		}
	} else {
		throw std::runtime_error("Reading OBJ file failed. This is usually because the operating system can't find it. Check if the relative path (to your terminal's working directory) is correct.");
	}

	return meshes;
}

void colourVertices(Mesh &VectorMesh, float4 colour) {
	VectorMesh.colours = std::vector<float>();
	VectorMesh.colours.resize(VectorMesh.vertexCount() * 4);
	for (unsigned int i = 0; i < VectorMesh.vertexCount()*4; i+=4) {
		VectorMesh.colours.at(i+0) = colour.x;
		VectorMesh.colours.at(i+1) = colour.y;
		VectorMesh.colours.at(i+2) = colour.z;
		VectorMesh.colours.at(i+3) = colour.w;
	}
}
void colourVertices(VectorMesh &VectorMesh, float4 colour) {
	VectorMesh.colours = std::vector<float4>();
	VectorMesh.colours.resize(VectorMesh.vertices.size());
	for (unsigned int i = 0; i < VectorMesh.vertices.size(); i++) {
		VectorMesh.colours[i] = colour;
	}
}

Mesh loadTerrainMesh(std::string const srcFile) {
	std::vector<VectorMesh> fileContents = loadWavefront(srcFile, true);
	Mesh terrainMesh = Mesh(fileContents.at(0));
	colourVertices(terrainMesh, float4(1, 1, 1, 1));

	return terrainMesh;
}

Helicopter loadHelicopterModel(std::string const srcFile) {
	std::vector<VectorMesh> fileContents = loadWavefront(srcFile, true);

	Helicopter out;

	for (VectorMesh VectorMesh : fileContents) {
		Mesh smesh = Mesh(VectorMesh);
		if(VectorMesh.name == "Body_body") {
			colourVertices(smesh, float4(0.3, 0.3, 0.3, 1.0));
			out.body = smesh;
		} else if(VectorMesh.name == "Main_Rotor_main_rotor") {
			colourVertices(smesh, float4(0.3, 0.1, 0.1, 1.0));
			out.mainRotor = smesh;
		} else if(VectorMesh.name == "Tail_Rotor_tail_rotor") {
			colourVertices(smesh, float4(0.1, 0.3, 0.1, 1.0));
			out.tailRotor = smesh;
		} else if(VectorMesh.name == "Door_door") {
			colourVertices(smesh, float4(0.1, 0.1, 0.3, 1.0));
			out.door = smesh;
		} else {
			throw std::runtime_error("The OBJ file did not contain any parts with names the loading function recognises. Did you load the correct OBJ file?");
		}
	}

	return out;
}
