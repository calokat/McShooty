#pragma once
#include "IInputSystem.h"
#include <map>
class LinuxInputSystem :
	public IInputSystem
{
public:
	LinuxInputSystem();
	void GetKeyPressed();
	glm::vec2 GetCursorPosition();
	glm::vec2 GetPreviousCursorPosition();
	glm::vec2 GetDeltaCursorPosition();
	bool IsKeyPressed(KeyboardCode kc);
	bool WasKeyPressed(KeyboardCode kc);
	bool IsMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonClicked(MouseButton mb);
	bool ShouldQuit();
private:
	InputData current;
	InputData previous;
	glm::vec2 deltaCursorPos;
	bool shouldQuit = false;
};
