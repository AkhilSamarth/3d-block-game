#pragma once

/* all non-graphics, logic stuff should go in here.
 * this will be called in a separate thread from main, where the rendering loop is.
 * contains the game loop.
 */
void startGame(GLFWwindow* window);