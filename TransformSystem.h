#pragma once
#include "Transform.h"
class TransformSystem
{
public:
	static void MoveRelative(glm::vec3 unrotated, Transform*);
	static void Rotate(glm::vec3 rotation, Transform*);
	static void SetOrientation(glm::vec3 rotation, Transform*);
	static void CalculateWorldMatrix(Transform* transform);
	static glm::vec3 CalculateForward(Transform* transform);
	static void CalculatePosition(Transform& transform);
	static void CalculateEulerAngles(Transform& transform);
	static void CalculateScale(Transform& transform);
	static glm::vec3 GetEulerRotation(Transform& transform);
	static void DecomposeTransform(Transform& out);
};

