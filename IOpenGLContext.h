#pragma once
class IOpenGLContext
{
public:
	virtual void* GetContext() = 0;
	virtual void _SwapBuffers(int winHandle) = 0;
	virtual ~IOpenGLContext(){};
};

