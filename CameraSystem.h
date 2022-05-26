#pragma once
#include "Camera.h"
#include "Transform.h"
class CameraSystem
{
public:
	static void CalculateViewMatrixRH(Camera& camera, Transform camTransform, bool transpose  = false);
	static void CalculateProjectionMatrixRH(Camera& camera, float aspect, bool transpose = false);
	static void CalculateViewMatrixLH(Camera& camera, Transform camTransform, bool transpose = false);
	static void CalculateProjectionMatrixLH(Camera& camera, float aspect, bool transpose = false);

};

