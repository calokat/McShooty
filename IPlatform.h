#pragma once
#include "IInputSystem.h"
class IPlatform
{
public:
	virtual int InitWindow() = 0;
	virtual long Run() = 0;
	virtual void* GetWindowHandle() = 0;
	virtual void* GetDeviceContext() = 0;
	virtual IInputSystem& GetInputSystem() = 0;
	virtual ~IPlatform() {};
};

