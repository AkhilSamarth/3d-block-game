#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	static Camera* activeCam;	// the camera which is currently outputting to the window

	glm::vec3 pos;		// position and forward direction of camera
	float pitch, yaw;	// rotation of camera (degrees), ranges: pitch: [-89, 89], yaw: [0, 360)
	float fov;		// this is the horizontal FOV, not the vertical! range: [30, 150]
public:
	static const float ASPECT_RATIO;

	static Camera* getActiveCam();	// return a pointer to the currently active camera

	Camera(glm::vec3 pos = glm::vec3(0, 0, 0), float pitch = 0.0f, float yaw = 0.0f, float fov = 90.0f);	// default camera is at position (0, 0, 0), facing towards -z, with 90 degree fov
	
	void activate();		// select this camera for outputting to the screen
	glm::mat4 getMatrix();	// returns the combined view and projection matrices of this camera
	
	void translate(glm::vec3 translation);		// translate the camera by the given vector
	void moveTo(glm::vec3 newPos);		// teleports the camera to newPos
	void rotateYaw(float angle);	// add angle to the current yaw
	void rotatePitch(float angle);	// add angle to the current pitch
	void setYaw(float angle);		// set the yaw to the given angle (0 = towards negative z, positive = counterclockwise)
	void setPitch(float angle);		// set the pitch to the given angle (positive = up)
	void setFov(float fov);		// set the horizontal fov
	
	// getters
	glm::vec3 getPosition();

	// calculate and return the forward, right, and up vectors (perpendicular to each other
	// vectors are normalized
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();

	float getYaw();
	float getPitch();
	float getFov();
};