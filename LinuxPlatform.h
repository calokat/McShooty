#pragma once
#include "IPlatform.h"
#include <EGL/egl.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>
#include <SDL/SDL.h>
#include "LinuxInputSystem.h"

class LinuxPlatform :
	public IPlatform
{
public:
	LinuxPlatform();
	~LinuxPlatform();
	int InitWindow();
	bool Run();
	SDL_Window* GetWindowHandle();
	void* GetDeviceContext();
	void NewGuiFrame();
	IInputSystem& GetInputSystem();
private:
	SDL_Window* g_Window = NULL;
	int shouldQuit = 0;
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;
	LinuxInputSystem inputSystem;
};
