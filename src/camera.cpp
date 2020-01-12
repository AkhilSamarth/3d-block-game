#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

#include <iostream>

const float Camera::ASPECT_RATIO = 16.0f / 9.0f;

Camera::Camera(glm::vec3 pos, glm::vec3 forward, float fov) : pos(pos), forward(forward), fov(fov) {}

glm::mat4 Camera::getMatrix() {
	// view matrix
	glm::mat4 view = glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0));

	// calculate vertical fov for GLM
	// calculation done on paper, only the result is used here
	float vertFov = 2 * glm::atan(glm::tan(glm::radians(fov) / 2) / ASPECT_RATIO);

	// projection matrix
	glm::mat4 projection = glm::perspective(vertFov, ASPECT_RATIO, 0.1f, 100.0f);

	return projection * view;
}