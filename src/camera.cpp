#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

#include <iostream>

const float Camera::ASPECT_RATIO = 16.0f / 9.0f;

Camera::Camera(glm::vec3 pos, float pitch, float yaw, float fov) : pos(pos), pitch(pitch), yaw(yaw), fov(fov) {}

glm::mat4 Camera::getMatrix() {
	glm::vec4 forward(0, 0, -1, 1);	// default forward vector

	// transform forward using yaw
	forward = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0)) * forward;

	// calculate right vector (axis of rotation for pitch)
	glm::vec3 right = glm::cross(glm::vec3(forward.x, forward.y, forward.z), glm::vec3(0, 1, 0));

	// transform forward using pitch
	forward = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right) * forward;

	// view matrix
	glm::mat4 view = glm::lookAt(pos, pos + glm::vec3(forward.x, forward.y, forward.z) , glm::vec3(0, 1, 0));

	// calculate vertical fov for GLM
	// calculation done on paper, only the result is used here
	float vertFov = 2 * glm::atan(glm::tan(glm::radians(fov) / 2) / ASPECT_RATIO);

	// projection matrix
	glm::mat4 projection = glm::perspective(vertFov, ASPECT_RATIO, 0.1f, 100.0f);

	return projection * view;
}

void Camera::translate(glm::vec3 translation) {
	pos += translation;
}

void Camera::moveTo(glm::vec3 newPos) {
	pos = newPos;
}

void Camera::rotateYaw(float angle) {
	yaw += angle;

	// keep yaw within 0 - 360 range
	yaw = glm::mod(yaw, 360.0f);
}

void Camera::rotatePitch(float angle) {
	pitch += angle;

	// keep pitch in range -89 - 89
	if (pitch > 89) {
		pitch = 89.0f;
	}
	else if (pitch < -89) {
		pitch = -89.0f;
	}
}

void Camera::setYaw(float angle) {
	yaw = angle;

	// keep yaw in range
	yaw = glm::mod(yaw, 360.0f);
}

void Camera::setPitch(float angle) {
	pitch = angle;

	// keep pitch in range
	if (pitch > 89) {
		pitch = 89.0f;
	}
	else if (pitch < -89) {
		pitch = -89.0f;
	}
}

void Camera::setFOV(float fov) {
	this->fov = fov;

	// keep fov in range
	if (fov > 150) {
		fov = 150.0f;
	}
	else if (fov < 30) {
		fov = 30.0f;
	}
}