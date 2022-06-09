#pragma once
#include "IInputSystem.h"
class IPlatform
{
public:
	virtual int InitWindow() = 0;
	virtual bool Run() = 0;
	virtual IInputSystem& GetInputSystem() = 0;
	virtual ~IPlatform() {};
};

