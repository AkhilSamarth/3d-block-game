#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "drawing.h"
#include "block.h"
#include "texture.h"
#include "camera.h"

// pointer to the camera that's currently being displayed
Camera* activeCam;

void mouseCallback(GLFWwindow* window, double x, double y) {
	static const float sensitivity = 0.08;

	// need to keep track of previous x and y to calculate deltas
	static double lastX = x;
	static double lastY = y;

	// calculate deltas
	double deltaX = x - lastX;
	double deltaY = y - lastY;

	// rotate camera
	activeCam->rotateYaw(-deltaX * sensitivity);
	activeCam->rotatePitch(-deltaY * sensitivity);

	// update "last" vars
	lastX = x;
	lastY = y;
}

void processKeys(GLFWwindow* window) {
	// how fast camera should move when a key is pressed
	static const float camSpeed = 0.01;

	// process key presses
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		activeCam->translate(activeCam->getForward() * camSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		activeCam->translate(activeCam->getForward() * -camSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		activeCam->translate(activeCam->getRight() * -camSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		activeCam->translate(activeCam->getRight() * camSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		activeCam->translate(activeCam->getUp() * camSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		activeCam->translate(activeCam->getUp() * -camSpeed);
	}
}

// select which camera to display to the window
void setActiveCamera(Camera* cam) {
	activeCam = cam;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// initalize glew
	glewInit();

	// mouse input setup
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);	// add mouse callback

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// load textures
	loadTexture("assetts/textures/test.png", "test");

	// create shader program
	Shader shader;
	shader.addShader("assetts/shaders/shader_vertex.glsl", GL_VERTEX_SHADER);
	shader.addShader("assetts/shaders/shader_fragment.glsl", GL_FRAGMENT_SHADER);
	shader.linkProgram();

	// test blocks
	std::vector<Block> blocks;
	blocks.push_back(Block(0, 0, -3, "test"));

	// create camera
	Camera cam;
	setActiveCamera(&cam);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// process input
		processKeys(window);

		// get camera matrix
		glm::mat4 camMatrix = activeCam->getMatrix();

		drawBlocks(blocks, shader.getProgramId(), camMatrix);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}