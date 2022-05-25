#include <stdio.h>
#include "WindowsPlatform.h"

int main(int argc, char* argv[])
{
    WindowsPlatform platform;
    platform.InitWindow();
    printf("Welcome to McShooty's\n");
    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) 
    {
        platform.Run();
    }
    return 0;
}