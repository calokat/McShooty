#pragma once
#include <Windows.h>

int InitWindow();
LRESULT EventLoop(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long Run();