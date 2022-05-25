#ifdef _WIN64
#pragma once
#include "IPlatform.h"
#include <Windows.h>
class WindowsPlatform :
	public IPlatform
{
public:
	WindowsPlatform();
	~WindowsPlatform();
	int InitWindow();
	unsigned int GetWindowWidth();
	unsigned int GetWindowHeight();
	HWND GetHwnd();
	long Run();
	void* GetWindowHandle();
	void* GetDeviceContext();
private:
	const int windowWidth = 800;
	const int windowHeight = 600;
	static WindowsPlatform* staticThis;
	HINSTANCE hInstance;
	static LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool hasFocus;
	HWND hwnd;
	HDC hdc;
};
#endif
