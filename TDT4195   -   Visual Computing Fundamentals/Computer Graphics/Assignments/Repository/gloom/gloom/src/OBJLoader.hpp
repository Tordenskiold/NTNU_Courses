
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "mesh.hpp"

struct Helicopter {
	Mesh body = Mesh("<missing>");
	Mesh mainRotor = Mesh("<missing>");
	Mesh tailRotor = Mesh("<missing>");
	Mesh door = Mesh("<missing>");
};

std::vector<VectorMesh> loadWavefront(std::string const srcFile, bool quiet = false);

Helicopter loadHelicopterModel(std::string const srcFile);
Mesh loadTerrainMesh(std::string const srcFile);