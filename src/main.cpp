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
	
}

void processKeys(GLFWwindow* window) {
	static const float camSpeed = 0.05;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
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

	// test block
	Block blocks[] = {
		Block(0, 0, -3, "test"),
	};

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

		drawBlocks(blocks, 1, shader.getProgramId(), camMatrix);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}