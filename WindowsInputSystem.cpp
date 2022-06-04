#ifdef _WIN64
#include "WindowsInputSystem.h"
#include <ctype.h>
WindowsInputSystem::WindowsInputSystem(HWND h) : hwnd(h)
{
	current = {};
	previous = {};
	cursor = GetCursor();
}
void WindowsInputSystem::GetKeyPressed()
{
	previous = current;

	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
	current.cursorPos = glm::vec2(mousePos.x, mousePos.y);

	current.keys[(unsigned long long)KeyboardCode::A] = GetAsyncKeyState('A') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::B] = GetAsyncKeyState('B') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::C] = GetAsyncKeyState('C') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::D] = GetAsyncKeyState('D') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::E] = GetAsyncKeyState('E') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::F] = GetAsyncKeyState('F') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::G] = GetAsyncKeyState('G') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::H] = GetAsyncKeyState('H') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::I] = GetAsyncKeyState('I') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::J] = GetAsyncKeyState('J') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::K] = GetAsyncKeyState('K') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::L] = GetAsyncKeyState('L') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::M] = GetAsyncKeyState('M') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::N] = GetAsyncKeyState('N') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::O] = GetAsyncKeyState('O') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::P] = GetAsyncKeyState('P') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Q] = GetAsyncKeyState('Q') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::R] = GetAsyncKeyState('R') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::S] = GetAsyncKeyState('S') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::T] = GetAsyncKeyState('T') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::U] = GetAsyncKeyState('U') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::V] = GetAsyncKeyState('V') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::W] = GetAsyncKeyState('W') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::X] = GetAsyncKeyState('X') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Y] = GetAsyncKeyState('Y') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Z] = GetAsyncKeyState('Z') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num0] = GetAsyncKeyState('0') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num1] = GetAsyncKeyState('1') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num2] = GetAsyncKeyState('2') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num3] = GetAsyncKeyState('3') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num4] = GetAsyncKeyState('4') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num5] = GetAsyncKeyState('5') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num6] = GetAsyncKeyState('6') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num7] = GetAsyncKeyState('7') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num8] = GetAsyncKeyState('8') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Num9] = GetAsyncKeyState('9') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::LCtrl] = GetAsyncKeyState(VK_LCONTROL) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::RCtrl] = GetAsyncKeyState(VK_RCONTROL) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::LShift] = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::RShift] = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::ForwardSlash] = GetAsyncKeyState('/') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::BackSlash] = GetAsyncKeyState('\\') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Enter] = GetAsyncKeyState('\n') & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Esc] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::LAlt] = GetAsyncKeyState(VK_LMENU) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::RAlt] = GetAsyncKeyState(VK_RMENU) & 0x8000;
	current.keys[(unsigned long long)KeyboardCode::Delete] = GetAsyncKeyState(VK_DELETE) & 0x8000;
	
	current.mouseButtons[(unsigned long long)MouseButton::Left] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	current.mouseButtons[(unsigned long long)MouseButton::Middle] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	current.mouseButtons[(unsigned long long)MouseButton::Right] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

glm::vec2 WindowsInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}

glm::vec2 WindowsInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}
glm::vec2 WindowsInputSystem::GetDeltaCursorPosition()
{
	return current.cursorPos - previous.cursorPos;
}
bool WindowsInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[(unsigned long long)kc];
}
bool WindowsInputSystem::WasKeyPressed(KeyboardCode kc)
{
	return previous.keys[(unsigned long long)kc];
}
bool WindowsInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[(unsigned long long)mb];
}
bool WindowsInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[(unsigned long long)mb];
}
bool WindowsInputSystem::WasMouseButtonClicked(MouseButton mb)
{
	return current.mouseButtons[(unsigned long long)MouseButton::Left] && !previous.mouseButtons[(unsigned long long)MouseButton::Left];
}
#endif