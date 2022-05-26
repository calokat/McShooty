#include "CameraSystem.h"
#include "TransformSystem.h"
void CameraSystem::CalculateViewMatrixRH(Camera& camera, Transform camTransform, bool transpose)
{
	glm::vec3 forward = TransformSystem::CalculateForward(&camTransform);

	if (transpose)
	{
		camera.view = glm::transpose(glm::lookAtRH(camTransform.position, camTransform.position + forward, glm::vec3(0, 1, 0)));
	}
	else
	{
		camera.view = glm::lookAtRH(camTransform.position, camTransform.position + forward, glm::vec3(0, 1, 0));
	}
}

void CameraSystem::CalculateProjectionMatrixRH(Camera& camera, float aspect, bool transpose)
{
	if (transpose)
	{
		camera.projection = glm::transpose(glm::perspective(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance));
	}
	else
	{
		camera.projection = glm::perspective(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance);
	}
}

void CameraSystem::CalculateViewMatrixLH(Camera& camera, Transform camTransform, bool transpose)
{
	glm::vec3 forward = TransformSystem::CalculateForward(&camTransform);

	if (transpose)
	{
		camera.view = glm::transpose(glm::lookAtLH(camTransform.position, camTransform.position + forward, glm::vec3(0, 1, 0)));
	}
	else
	{
		camera.view = glm::lookAtLH(camTransform.position, camTransform.position + forward, glm::vec3(0, 1, 0));
	}
}

void CameraSystem::CalculateProjectionMatrixLH(Camera& camera, float aspect, bool transpose)
{
	if (transpose)
	{
		camera.projection = glm::transpose(glm::perspectiveLH(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance));
	}
	else
	{
		camera.projection = glm::perspectiveLH(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance);
	}
}
