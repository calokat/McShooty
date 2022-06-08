#include "Camera.h"

Camera::Camera()
{
	nearPlaneDistance = 0;
	farPlaneDistance = 0;
	movementSpeed = 0;
	lookSpeed = 0;
	fieldOfView = 0;
	aspectRatio = 0;
	prevMousePosition = { -1, -1 };
	projection = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
}

Camera::Camera(float aspectRatio)
{
	nearPlaneDistance = .1f;
	farPlaneDistance = 1000.0f;
	movementSpeed = 10;
	lookSpeed = 1;
	fieldOfView = 3.14f / 3;
	this->aspectRatio = aspectRatio;
	prevMousePosition = glm::vec2();
	prevMousePosition.x = -1;
	prevMousePosition.y = -1;
}
