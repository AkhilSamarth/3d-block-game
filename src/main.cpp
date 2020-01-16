#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <thread>

#include "drawing.h"
#include "block.h"
#include "texture.h"
#include "camera.h"
#include "game.h"

#define FPS_COUNTER_INTERVAL 0.5	// how often (in seconds) to print FPS

static const bool showFPS = true;	// whether or not to print the FPS

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

	// enable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// load textures
	loadTexture("assetts/textures/test.png", "test");

	// create shader program
	Shader shader;
	shader.addShader("assetts/shaders/shader_vertex.glsl", GL_VERTEX_SHADER);
	shader.addShader("assetts/shaders/shader_fragment.glsl", GL_FRAGMENT_SHADER);
	shader.linkProgram();

	// test blocks
	std::vector<Block> blocks;
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			blocks.push_back(Block(i, 0, -j, "test"));
		}
	}

	// create and activate camera
	Camera cam;
	cam.activate();

	// start game loop
	std::thread gameThread(startGame, window);

	// timer for fps counter
	double fpsTimer = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		double renderStartTime = glfwGetTime();		// used to calculate how long each cycle of render loop took

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// get camera matrix
		glm::mat4 camMatrix = Camera::getActiveCam()->getMatrix();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		// update FPS timer if needed
		if (showFPS && (glfwGetTime() - fpsTimer >= FPS_COUNTER_INTERVAL)) {
			printf("FPS: %f, sec per frame (1/FPS): %f\n", 1.0f / ((glfwGetTime() - renderStartTime)), (glfwGetTime() - renderStartTime));
			fpsTimer = glfwGetTime();
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	// wait for game loop to end
	gameThread.join();

	glfwTerminate();
	return 0;
}