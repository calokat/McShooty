#pragma once
#ifdef _WIN64
#include <Windows.h>
LRESULT EventLoop(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#ifdef __linux__
#include <SDL2/SDL.h>
bool EventLoop();
#endif
int InitWindow();
long Run();