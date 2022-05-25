#pragma once
class IGraphicsAPI
{
public:
	virtual int Init() = 0;
	virtual void ClearScreen() = 0;
	virtual void _SwapBuffers() = 0;
	virtual void NewGuiFrame() = 0;
	virtual void DrawGui() = 0;
	virtual ~IGraphicsAPI() {};
	virtual void BindToScreen() = 0;
};