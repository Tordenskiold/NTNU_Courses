#include <iostream>
#include <chrono>
#include <fstream>
#include "toolbox.hpp"
#include <glm/gtx/transform.hpp>

// The standard library's random number generator needs to be seeded in order to produce
// different results every time the program is run. This should only happen once, so
// we keep track here with a global variable whether this has happened previously.
bool isRandomInitialised = false;

float randomUniformFloat() {
    if (!isRandomInitialised) {
        // Initialise the random number generator using the current time as a seed
        srand(static_cast <unsigned> (time(0)));
        isRandomInitialised = true;
    }
    // rand() produces a random integer between 0 and RAND_MAX. This normalises it to a number between 0 and 1.
    return static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
}

// In order to be able to calculate when the getTimeDeltaSeconds() function was last called, we need to know the point in time when that happened. This requires us to keep hold of that point in time.
// We initialise this value to the time at the start of the program.
static std::chrono::steady_clock::time_point _previousTimePoint = std::chrono::steady_clock::now();

// Calculates the elapsed time since the previous time this function was called.
double getTimeDeltaSeconds() {
    // Determine the current time
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    // Calculate the number of nanoseconds that elapsed since the previous call to this function
    long long timeDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - _previousTimePoint).count();
    // Convert the time delta in nanoseconds to seconds
    double timeDeltaSeconds = (double)timeDelta / 1000000000.0;

    // Store the previously measured current time
    _previousTimePoint = currentTime;

    // Return the calculated time delta in seconds
    return timeDeltaSeconds;
}

Heading simpleHeadingAnimation(double time) {
    // Constants
    const float step = 0.05;
    const float pathSize = 15;
    const float circuitSpeed = 0.8f;

    // Compute finite difference to approximate tangent for heading direction
    float xpos = pathSize*sin(2*time*circuitSpeed);
    float nextxpos = pathSize*sin(2*(time+step)*circuitSpeed);
    float zpos = 3*pathSize*cos(time*circuitSpeed);
    float nextzpos = 3*pathSize*cos((time+step)*circuitSpeed);
    glm::vec2 dpos = glm::vec2(nextxpos-xpos, nextzpos-zpos);

    double angle = glm::pi<double>() + atan2(dpos.x, dpos.y);

    double rollAngle = cos(time*circuitSpeed);

    Heading heading = Heading();

    heading.x = xpos;
    heading.z = zpos;
    heading.yaw = angle;
    heading.pitch = glm::radians(-10.0 * glm::length(dpos));
    heading.roll = glm::radians(rollAngle * 30.0);

    return heading;
}
