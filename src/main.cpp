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
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			blocks.push_back(Block(i, 0, -j, "test"));
		}
	}

	// create and activate camera
	Camera cam;
	cam.activate();

	// start game loop
	std::thread gameThread(startGame, window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		double lastTime = glfwGetTime();		// start timer
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// get camera matrix
		glm::mat4 camMatrix = Camera::getActiveCam()->getMatrix();

		drawBlocks(blocks, shader.getProgramId(), camMatrix, "test");
		
		// update FPS timer if needed
		if (showFPS) {
			printf("FPS: %f, 1/FPS: %f\n", 1.0f / ((glfwGetTime() - lastTime)), (glfwGetTime() - lastTime));
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// wait for game loop to end
	gameThread.join();

	glfwTerminate();
	return 0;
}