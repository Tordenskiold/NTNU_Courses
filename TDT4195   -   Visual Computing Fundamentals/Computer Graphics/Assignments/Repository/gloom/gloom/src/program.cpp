// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include <vector>
#include <gloom\shader.hpp>
#include <iostream>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>
#include <mesh.hpp>
#include <OBJLoader.hpp>
#include <toolbox.hpp>

using namespace std;



glm::vec3 camRotation = glm::vec3(0.24f, 0.00f, 0.00f);
glm::vec3 camPosition = glm::vec3(0.00f, -22.00f, -116.00f);

double mainDegrees = 0.0f;
double tailDegrees = 0.0f;
int mainRPM = 560.0f;
int tailRPM = 6 * mainRPM;

double deltaTime;
double totalTime = 0.0f;

int helis = 5;
double interval = 0.75f;

vector<Heading> headings(helis);



void runProgram(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Gloom::Shader shader;
	shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");

	SceneNode* root = setupSceneGraph(helis);

	glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 10000.0f);

	shader.activate();
	while (!glfwWindowShouldClose(window)) {

		deltaTime = getTimeDeltaSeconds();
		totalTime += deltaTime;
		mainDegrees = fmod((deltaTime * (mainRPM / 60) * 360) + mainDegrees, 360);
		tailDegrees = fmod((deltaTime * (tailRPM / 60) * 360) + tailDegrees, 360);

		for (int i = 0; i < helis; i++)
			headings[i] = simpleHeadingAnimation(totalTime + (i * interval));

		updateHeliStates(root, headings, mainDegrees, tailDegrees);

		cout << camPosition.z << endl;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 translation = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(camPosition.x, camPosition.y, camPosition.z, 1));
		glm::mat4 rotationX = glm::rotate(camRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotationY = glm::rotate(camRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationZ = glm::rotate(camRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 transformation = perspectiveMatrix * rotationX * rotationY * rotationZ * translation;

		updateSceneNode(root, glm::mat4(1.0));
		drawSceneNode(root, transformation);



		glfwPollEvents();
		handleKeyboardInput(window);

		glfwSwapBuffers(window);

	}
	shader.deactivate();
	shader.destroy();

	printGLError();

}





int setupVAO(vector<float> vertices, vector<float> normals, vector<float> colors, vector<unsigned int> indices) {
	GLuint arrayID = 0;
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);

	vector<float> values = {};
	int i = 0, j = 0, k = 0, l;
	int q;
	for (l = 0; l < vertices.size() / 3; l++) {
		for (q = 0; q < 3; q++)
			values.push_back(vertices.at(i++));
		for (q = 0; q < 4; q++)
			values.push_back(colors.at(j++));
		for (q = 0; q < 3; q++)
			values.push_back(normals.at(k++));
	}

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(float), values.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (char*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (char*)(sizeof(float) * 7));

	GLuint indexID;
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	return arrayID;
}



void updateHeliStates(SceneNode* node, vector<Heading> headings, int main, int tail) {
	// Task 4B - heliBodyID is added to the SceneNode-objects (defaults to -1)
	if (node->heliBodyID > -1) {
		node->position.x = headings.at(node->heliBodyID).x;
		node->position.z = headings.at(node->heliBodyID).z;
		node->rotation.x = headings.at(node->heliBodyID).pitch;
		node->rotation.y = headings.at(node->heliBodyID).yaw;
		node->rotation.z = headings.at(node->heliBodyID).roll;
	}
	if (node->mainRotor)
		node->rotation.y = (mainDegrees * 3.14f) / 180.0f;
	if (node->tailRotor)
		node->rotation.x = (tailDegrees * 3.14f) / 180.0f;

	for (SceneNode* child : node->children)
		updateHeliStates(child, headings, main, tail);
}



void drawSceneNode(SceneNode* node, glm::mat4x4 viewProjectionMatrix) {
	if (node->vertexArrayObjectID != -1) {

		glm::mat4 transform = viewProjectionMatrix * node->currentTransformationMatrix;
		glBindVertexArray(node->vertexArrayObjectID);
		glUniformMatrix4fv(5, 1, GL_FALSE, (GLfloat*)& transform[0][0]);

		// Task 5B
		glUniformMatrix4fv(6, 1, GL_FALSE, (GLfloat*)& node->currentTransformationMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0);
	}
	for (SceneNode* child : node->children)
		drawSceneNode(child, viewProjectionMatrix);
}



void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {

	glm::mat4 reference = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(node->referencePoint.x, node->referencePoint.y, node->referencePoint.z, 1));
	glm::mat4 referenceInverse = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(-node->referencePoint.x, -node->referencePoint.y, -node->referencePoint.z, 1));


	glm::mat4 translation = glm::mat4x4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(node->position.x, node->position.y, node->position.z, 1));
	glm::mat4 rotationX = glm::rotate(node->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(node->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZ = glm::rotate(node->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	transformationThusFar *= reference * translation * rotationX * rotationY * rotationZ * referenceInverse;

	node->currentTransformationMatrix = transformationThusFar;

	for (SceneNode* child : node->children)
		updateSceneNode(child, transformationThusFar);
}



SceneNode* setupSceneGraph(int helis) {
	Mesh moonMesh = loadTerrainMesh("../gloom/resources/lunarsurface.obj");
	GLuint moonID = setupVAO(moonMesh.vertices, moonMesh.normals, moonMesh.colours, moonMesh.indices);

	SceneNode* root = createSceneNode();
	SceneNode* moon = createSceneNode();
	addChild(root, moon);

	Helicopter heliMesh = loadHelicopterModel("../gloom/resources/helicopter.obj");
	for (int i = 0; i < helis; i++) {
		GLuint bodyID = setupVAO(heliMesh.body.vertices, heliMesh.body.normals, heliMesh.body.colours, heliMesh.body.indices);
		GLuint mainID = setupVAO(heliMesh.mainRotor.vertices, heliMesh.mainRotor.normals, heliMesh.mainRotor.colours, heliMesh.mainRotor.indices);
		GLuint tailID = setupVAO(heliMesh.tailRotor.vertices, heliMesh.tailRotor.normals, heliMesh.tailRotor.colours, heliMesh.tailRotor.indices);
		GLuint doorID = setupVAO(heliMesh.door.vertices, heliMesh.door.normals, heliMesh.door.colours, heliMesh.door.indices);
		SceneNode* body = createSceneNode();
		SceneNode* main = createSceneNode();
		SceneNode* tail = createSceneNode();
		SceneNode* door = createSceneNode();
		addChild(moon, body);
		addChild(body, main);
		addChild(body, tail);
		addChild(body, door);

		body->vertexArrayObjectID = bodyID;
		main->vertexArrayObjectID = mainID;
		tail->vertexArrayObjectID = tailID;
		door->vertexArrayObjectID = doorID;

		body->VAOIndexCount = heliMesh.body.vertices.size();
		main->VAOIndexCount = heliMesh.mainRotor.vertices.size();
		tail->VAOIndexCount = heliMesh.tailRotor.vertices.size();
		door->VAOIndexCount = heliMesh.door.vertices.size();

		body->heliBodyID = i;
		main->mainRotor = true;
		tail->tailRotor = true;

		tail->referencePoint = glm::vec3(0.35f, 2.3f, 10.4f);
	}

	root->vertexArrayObjectID = -1;
	moon->vertexArrayObjectID = moonID;
	moon->VAOIndexCount = moonMesh.vertices.size();
	return root;
}






void handleKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	float rotationSpeed = 0.03f;
	float movementSpeed = 2.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camRotation.x -= rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camRotation.x += rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camRotation.y -= rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camRotation.y += rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camRotation.z -= rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camRotation.z += rotationSpeed;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) camRotation.z = 0;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camPosition.x -= movementSpeed * sin(camRotation.y);
		camPosition.z += movementSpeed * cos(camRotation.y);
		//camPosition.z += movementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camPosition.x += movementSpeed * sin(camRotation.y);
		camPosition.z -= movementSpeed * cos(camRotation.y);
		//camPosition.z -= movementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camPosition.x += movementSpeed * cos(camRotation.y);
		camPosition.z += movementSpeed * sin(camRotation.y);
		//camPosition.x += movementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camPosition.x -= movementSpeed * cos(camRotation.y);
		camPosition.z -= movementSpeed * sin(camRotation.y);
		//camPosition.x -= movementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camPosition.y += movementSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camPosition.y -= movementSpeed;
}
