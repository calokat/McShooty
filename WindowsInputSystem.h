#ifdef _WIN64
#pragma once
#include "IInputSystem.h"
#include <entt/entt.hpp>
#include <vector>
#include <functional>
#include <Windows.h>


struct InputFunction 
{
	std::function<void()> func;
	char key;
};

class WindowsInputSystem :
	public IInputSystem
{
public:
	WindowsInputSystem(HWND h);
	void GetKeyPressed();
	glm::vec2 GetCursorPosition();
	glm::vec2 GetPreviousCursorPosition();
	glm::vec2 GetDeltaCursorPosition();
	bool IsKeyPressed(KeyboardCode kc);
	bool WasKeyPressed(KeyboardCode kc);
	bool IsMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonClicked(MouseButton mb);
	InputData current;
	InputData previous;

private:
	HWND hwnd;
	HCURSOR cursor;
};
#endif