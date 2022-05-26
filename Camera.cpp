#include "Camera.h"
#include <iostream>

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
	//transform = new Transform();
	nearPlaneDistance = .1f;
	farPlaneDistance = 1000.0f;
	movementSpeed = 10;
	lookSpeed = 1;
	fieldOfView = 3.14f / 3;
	//transform->SetPosition(initialPos);
	//UpdateViewMatrix();
	this->aspectRatio = aspectRatio;
	//UpdateProjectionMatrix(this->aspectRatio);
	prevMousePosition = glm::vec2();
	prevMousePosition.x = -1;
	prevMousePosition.y = -1;
	//XMStoreFloat4x4(&this->view, XMMatrixIdentity());
	//XMStoreFloat4x4(&this->projection, XMMatrixIdentity());
}
//glm::mat4 Camera::GetViewMatrix()
//{
//	UpdateViewMatrix();
//	return this->view;
//}
//glm::mat4 Camera::GetProjectionMatrix()
//{
//	return this->projection;
//}
//void Camera::UpdateProjectionMatrix(float aspect)
//{
//	//XMStoreFloat4x4(&this->projection, XMMatrixPerspectiveFovLH(fieldOfView, aspect, nearPlaneDistance, farPlaneDistance));
//	this->projection = glm::perspective(fieldOfView, aspect, nearPlaneDistance, farPlaneDistance);
//}

//void Camera::UpdateViewMatrix()
//{
//	//XMVECTOR position = XMVectorSet(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z, 0);
//	//XMVECTOR forward = XMVectorSet(transform.GetForward().x, transform.GetForward().y, transform.GetForward().z, 0);
//	//XMStoreFloat4x4(&this->view, XMMatrixLookToLH(position, forward, XMVectorSet(0, 1, 0, 0)));
//	//glm::vec3 position(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
//	//glm::vec3 forward(transform.GetForward().x, transform.GetForward().y, transform.GetForward().z);
//	glm::vec3 forward = this->transform->GetForward();
//	this->view = glm::lookAtRH(this->transform->GetPosition(), this->transform->GetPosition() + this->transform->GetForward(), glm::vec3(0, 1, 0));
//}

//void Camera::Update(float dt, HWND windowHandle)
//{
//	if (GetAsyncKeyState('W') & 0x8000)
//	{
//		transform->MoveRelative(glm::vec3(0, 0, dt * movementSpeed));
//	}
//	if (GetAsyncKeyState('S') & 0x8000)
//	{
//		transform->MoveRelative(glm::vec3(0, 0, -dt * movementSpeed));
//	}
//	if (GetAsyncKeyState('A') & 0x8000)
//	{
//		transform->MoveRelative(glm::vec3(dt * movementSpeed, 0, 0));
//	}
//	if (GetAsyncKeyState('D') & 0x8000)
//	{
//		transform->MoveRelative(glm::vec3(-dt * movementSpeed, 0, 0));
//	}
//	if (GetAsyncKeyState('Q') & 0x8000)
//	{
//		transform->MoveAbsolute(glm::vec3(0, -dt * movementSpeed, 0));
//	}
//	if (GetAsyncKeyState('E') & 0x8000)
//	{
//		transform->MoveAbsolute(glm::vec3(0, dt * movementSpeed, 0));
//	}
//
//	// mouse input
//	POINT mousePos = {};
//	GetCursorPos(&mousePos);
//	ScreenToClient(windowHandle, &mousePos);
//	if (prevMousePosition.x == -1)
//	{
//		prevMousePosition.x = mousePos.x;
//		prevMousePosition.y = mousePos.y;
//	}
//	float mouseMoveDeltaX, mouseMoveDeltaY;
//	mouseMoveDeltaX = (mousePos.x - prevMousePosition.x) * lookSpeed * dt;
//	mouseMoveDeltaY = (mousePos.y - prevMousePosition.y) * lookSpeed * dt;
//	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
//	{
//		transform->Rotate(glm::vec3(mouseMoveDeltaY, -mouseMoveDeltaX, 0));
//	}
//	UpdateViewMatrix();
//	prevMousePosition = mousePos;
//}

//Transform* Camera::GetTransform()
//{
//	return transform;
//}

//Camera& Camera::operator=(Camera&& other) noexcept
//{
//	if (this != &other)
//	{
//		this->view = other.view;
//		this->projection = other.projection;
//		this->fieldOfView = other.fieldOfView;
//		this->aspectRatio = other.aspectRatio;
//		this->nearPlaneDistance = other.nearPlaneDistance;
//		this->farPlaneDistance = other.farPlaneDistance;
//		this->movementSpeed = other.movementSpeed;
//		this->lookSpeed = other.lookSpeed;
//		this->prevMousePosition = other.prevMousePosition;
//		//this->transform = new Transform((*other.transform));
//	}
//	return *this;
//}
//
//Camera::Camera(Camera&& other) noexcept
//{
//	*this = std::move(other);
//}
//
//Camera::Camera(Camera& other)
//{
//	*this = std::move(other);
//}

//Camera::~Camera()
//{
//	//delete transform;
//}