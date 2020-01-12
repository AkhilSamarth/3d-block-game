#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 pos, forward;		// position and forward direction of camera
	float fov;		// this is the horizontal FOV, not the vertical!
public:
	glm::mat4 getMatrix();	// returns the combined view and projection matrices of this camera
	void translate(glm::vec3 translation);		// translate the camera
	void moveTo(glm::vec3 newPos);		// teleports the camera to newPos
	void rotateYaw(float angle);	// add angle to the current yaw
	void rotatePitch(float angle);	// add angle to the current pitch
	void setYaw(float angle);		// set the yaw to the given angle (0 = towards negative z, default)
	void setPitch(float angle);		// set the pitch to the given angle (positive = up, must be within range -89 to 89, inclusive)
	void setFOV(float fov);		// set the horizontal fov
};