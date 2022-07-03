#if  defined(__linux__) || defined(__EMSCRIPTEN__)
#include "LinuxPlatform.h"
#include <GL/glew.h>
#include <stdio.h>
LinuxPlatform::LinuxPlatform()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return;
	}
}

LinuxPlatform::~LinuxPlatform()
{
}

int LinuxPlatform::InitWindow()
{
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
	g_Window = SDL_CreateWindow("Portable Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, window_flags);

	SDL_RaiseWindow(g_Window);
	return 0;
}

bool LinuxPlatform::Run()
{
	inputSystem.GetKeyPressed();
	return inputSystem.ShouldQuit();
}

SDL_Window* LinuxPlatform::GetWindowHandle()
{
	return g_Window;
}
void* LinuxPlatform::GetDeviceContext()
{
	return g_Window;
}
void LinuxPlatform::NewGuiFrame()
{
}

IInputSystem& LinuxPlatform::GetInputSystem()
{
	return inputSystem;
}
#endif