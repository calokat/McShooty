#pragma once
#include "glm/glm.hpp"
//#include <d3d11.h>
//#include <math.h>
class Camera
{
private:
	//Transform* transform;
	//void UpdateViewMatrix();
public:
	glm::mat4 view;
	glm::mat4 projection;
	float fieldOfView;
	float aspectRatio;
	float nearPlaneDistance;
	float farPlaneDistance;
	float movementSpeed;
	float lookSpeed;
	glm::vec2 prevMousePosition;
	Camera();
	Camera(float aspectRatio);
	//glm::mat4 GetViewMatrix();
	//glm::mat4 GetProjectionMatrix();
	//void UpdateProjectionMatrix(float aspect);
	//void Update(float dt, HWND windowHandle);
	//Transform* GetTransform();
	//Camera& operator=(Camera&& other) noexcept;
	//Camera(Camera&& other) noexcept;
	//Camera(Camera& other);
	//~Camera();
};

