#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <sceneGraph.hpp>
#include <toolbox.hpp>

using namespace std;






int setupVAO(vector<float> vertices, vector<float> normals, vector<float> colors, vector<unsigned int> indices);

void drawSceneNode(SceneNode* node, glm::mat4x4 viewProjectionMatrix);

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar);

SceneNode* setupSceneGraph(int helis);

void updateHeliStates(SceneNode* node, vector<Heading> headings, int main, int tail);



// Main OpenGL program
void runProgram(GLFWwindow* window);


// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window);


// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
inline void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}


#endif
