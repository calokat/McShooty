#pragma once
class IOpenGLContext
{
public:
	virtual void _SwapBuffers() = 0;
	virtual ~IOpenGLContext(){};
};

