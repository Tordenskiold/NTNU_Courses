#pragma once

#include <glm/mat4x4.hpp>
#include "mesh.hpp"

// Generates a mesh containing a 3D object which looks like a chessboard.
// Mesh generateChessboard(unsigned int width, unsigned int height, float tileWidth, float4 tileColour1, float4 tileColour2);

// Returns a random float between 0 and 1
float randomUniformFloat();

// Return the amount of time elapsed since the LAST TIME this function was called, in seconds.
double getTimeDeltaSeconds();

struct Heading {
    float x;
    float z;
    float yaw;
    float pitch;
    float roll;
};

// Returns a Heading animated by incrementing the time argument
Heading simpleHeadingAnimation(double time);
