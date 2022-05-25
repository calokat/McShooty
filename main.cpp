#include <stdio.h>
#include "WindowsPlatform.h"
#include "OpenGLAPI.h"
int main(int argc, char* argv[])
{
    WindowsPlatform platform;
    OpenGLAPI graphics(&platform);
    platform.InitWindow();
    graphics.Init();
    printf("Welcome to McShooty's\n");
    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) 
    {
        platform.Run();
        graphics.ClearScreen();
        graphics._SwapBuffers();
    }
    return 0;
}