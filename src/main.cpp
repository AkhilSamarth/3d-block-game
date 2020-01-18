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
#include "chunk.h"

#define SHOW_FPS true
#define FPS_COUNTER_INTERVAL 0.5	// how often (in seconds) to print FPS

int main(void) {
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

	// set clear color to sky blue
	glClearColor(0, 0.765, 1, 1);

	// load textures
	Texture::loadTextures();

	// create shader program
	Shader shader;
	shader.addShader("assetts/shaders/shader_vertex.glsl", GL_VERTEX_SHADER);
	shader.addShader("assetts/shaders/shader_fragment.glsl", GL_FRAGMENT_SHADER);
	shader.linkProgram();
	
	// test blocks
	for (int y = 0; y < 5; y++) {
		std::string blockName;
		if (y == 0 || y == 1) {
			blockName = "stone";
		}
		else if (y == 2 || y == 3) {
			blockName = "dirt";
		}
		else {
			blockName = "grass";
		}
		for (int x = -25; x < 25; x++) {
			for (int z = -25; z < 25; z++) {
				Chunk::addBlock(blockName, x, y, z);
			}
		}
	}
	
	// create and activate camera
	Camera cam;
	cam.activate();

	// start game loop
	std::thread* gameThread = startGame(window);

	// timer for fps counter
	double fpsTimer = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		double renderStartTime = glfwGetTime();		// used to calculate how long each cycle of render loop took

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get camera matrix
		glm::mat4 camMatrix = Camera::getActiveCam()->getMatrix();

		// draw chunks
		drawChunks(shader.getProgramId(), camMatrix);
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		// update FPS timer if needed
		if (SHOW_FPS && (glfwGetTime() - fpsTimer >= FPS_COUNTER_INTERVAL)) {
			printf("FPS: %f, ms per frame: %f\n", 1.0f / ((glfwGetTime() - renderStartTime)), (glfwGetTime() - renderStartTime) * 1000);
			fpsTimer = glfwGetTime();
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	// wait for game loop to end
	gameThread->join();
	delete gameThread;

	glfwTerminate();
	return 0;
}