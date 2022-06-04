#ifdef __linux__
#include "LinuxInputSystem.h"
#include <iostream>
#include <SDL2/SDL.h>

LinuxInputSystem::LinuxInputSystem()
{
}

void LinuxInputSystem::GetKeyPressed()
{
	previous = current;
	SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // Capture events here, based on io.WantCaptureMouse and io.WantCaptureKeyboard
		//printf("Key is %i and KeyCode is %i\n", event.key.keysym.scancode, sdlKeyMap[event.key.keysym.scancode]);
		//current.keys[sdlKeyMap[event.key.keysym.scancode]] = (event.type == SDL_KEYDOWN);
		if (event.type == SDL_EventType::SDL_KEYDOWN || event.type == SDL_EventType::SDL_KEYUP)
		{
			KeyboardCode keyCode;
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_0:
				keyCode = KeyboardCode::Num0;
				break;
			case SDL_SCANCODE_1:
				keyCode = KeyboardCode::Num1;
				break;
			case SDL_SCANCODE_2:
				keyCode = KeyboardCode::Num2;
				break;
			case SDL_SCANCODE_3:
				keyCode = KeyboardCode::Num3;
				break;
			case SDL_SCANCODE_4:
				keyCode = KeyboardCode::Num4;
				break;
			case SDL_SCANCODE_5:
				keyCode = KeyboardCode::Num5;
				break;
			case SDL_SCANCODE_6:
				keyCode = KeyboardCode::Num6;
				break;
			case SDL_SCANCODE_7:
				keyCode = KeyboardCode::Num7;
				break;
			case SDL_SCANCODE_8:
				keyCode = KeyboardCode::Num8;
				break;
			case SDL_SCANCODE_9:
				keyCode = KeyboardCode::Num9;
				break;
			case SDL_SCANCODE_A:
				keyCode = KeyboardCode::A;
				break;
			case SDL_SCANCODE_B:
				keyCode = KeyboardCode::B;
				break;
			case SDL_SCANCODE_C:
				keyCode = KeyboardCode::C;
				break;
			case SDL_SCANCODE_D:
				keyCode = KeyboardCode::D;
				break;
			case SDL_SCANCODE_E:
				keyCode = KeyboardCode::E;
				break;
			case SDL_SCANCODE_F:
				keyCode = KeyboardCode::F;
				break;
			case SDL_SCANCODE_G:
				keyCode = KeyboardCode::G;
				break;
			case SDL_SCANCODE_H:
				keyCode = KeyboardCode::H;
				break;
			case SDL_SCANCODE_I:
				keyCode = KeyboardCode::I;
				break;
			case SDL_SCANCODE_J:
				keyCode = KeyboardCode::J;
				break;
			case SDL_SCANCODE_K:
				keyCode = KeyboardCode::K;
				break;
			case SDL_SCANCODE_L:
				keyCode = KeyboardCode::L;
				break;
			case SDL_SCANCODE_M:
				keyCode = KeyboardCode::M;
				break;
			case SDL_SCANCODE_N:
				keyCode = KeyboardCode::N;
				break;
			case SDL_SCANCODE_O:
				keyCode = KeyboardCode::O;
				break;
			case SDL_SCANCODE_P:
				keyCode = KeyboardCode::P;
				break;
			case SDL_SCANCODE_Q:
				keyCode = KeyboardCode::Q;
				break;
			case SDL_SCANCODE_R:
				keyCode = KeyboardCode::R;
				break;
			case SDL_SCANCODE_S:
				keyCode = KeyboardCode::S;
				break;
			case SDL_SCANCODE_T:
				keyCode = KeyboardCode::T;
				break;
			case SDL_SCANCODE_U:
				keyCode = KeyboardCode::U;
				break;
			case SDL_SCANCODE_V:
				keyCode = KeyboardCode::V;
				break;
			case SDL_SCANCODE_W:
				keyCode = KeyboardCode::W;
				break;
			case SDL_SCANCODE_X:
				keyCode = KeyboardCode::X;
				break;
			case SDL_SCANCODE_Y:
				keyCode = KeyboardCode::Y;
				break;
			case SDL_SCANCODE_Z:
				keyCode = KeyboardCode::Z;
				break;
			case SDL_SCANCODE_DELETE:
				keyCode = KeyboardCode::Delete;
				break;
			case SDL_SCANCODE_LALT:
				keyCode = KeyboardCode::LAlt;
				break;
			case SDL_SCANCODE_RALT:
				keyCode = KeyboardCode::RAlt;
				break;
			case SDL_SCANCODE_LSHIFT:
				keyCode = KeyboardCode::LShift;
				break;
			case SDL_SCANCODE_RSHIFT:
				keyCode = KeyboardCode::RShift;
				break;
			case SDL_SCANCODE_ESCAPE:
				keyCode = KeyboardCode::Esc;
				break;
			default:
				keyCode = KeyboardCode::N;
				break;
			}
			current.keys[(int)keyCode] = event.type == SDL_KEYDOWN;
			current.keys[(int)KeyboardCode::LCtrl] = current.keys[(int)KeyboardCode::RCtrl] = SDL_GetModState() & KMOD_CTRL;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				current.mouseButtons[(int)MouseButton::Left] = true;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				current.mouseButtons[(int)MouseButton::Right] = true;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				current.mouseButtons[(int)MouseButton::Left] = false;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				current.mouseButtons[(int)MouseButton::Right] = false;
			}
		}
		// Some credit to https://github.com/ocornut/imgui/blob/master/examples/example_sdl_opengl3/main.cpp#L113
		// TODO: check window ID before closing
		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			shouldQuit = 1;
		}
    }
	 int mx, my;
	 SDL_GetMouseState(&mx, &my);
	 current.cursorPos.x = (float)mx;
	 current.cursorPos.y = (float)my;

}
bool LinuxInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[(int)kc];
}

bool LinuxInputSystem::WasKeyPressed(KeyboardCode kc)
{
	return previous.keys[(int)kc];
}

bool LinuxInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[(int)mb];
}

bool LinuxInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[(int)mb];
}

bool LinuxInputSystem::WasMouseButtonClicked(MouseButton mb)
{
	return previous.mouseButtons[(int)MouseButton::Left] && !current.mouseButtons[(int)MouseButton::Left];
}

glm::vec2 LinuxInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}
glm::vec2 LinuxInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}

glm::vec2 LinuxInputSystem::GetDeltaCursorPosition()
{
	// Hack: deltaCursorPos does not become (0, 0) when the mouse stops moving. When the mouse stops moving, MouseCallback is not called,
	// so deltaCursorPos is not assigned (0, 0) and it retains its non-zero value from the previous frame. This hack ensures that 
	// deltaCursorPos is (0, 0) when the mouse is not moving by manually zeroing it and letting that value be overridden in MouseCallback
	// when called.
	// glm::vec2 returnValue = deltaCursorPos;
	// deltaCursorPos = glm::vec2(0, 0);
	return current.cursorPos - previous.cursorPos;	
}

int LinuxInputSystem::ShouldQuit()
{
	return shouldQuit;
}
#endif