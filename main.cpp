#include <stdio.h>
#include "WindowsPlatform.h"
#include "OpenGLAPI.h"
#include "MeshLoaderSystem.h"
int main(int argc, char* argv[])
{
    WindowsPlatform platform;
    OpenGLAPI graphics(&platform);
    platform.InitWindow();
    graphics.Init();
    printf("Welcome to McShooty's\n");
    Mesh spiral;
    MeshLoaderSystem::ProcessMesh("C:\\Users\\Caleb\\Documents\\GitHub\\PortableEngine\\Assets\\Models\\helix.obj", spiral);
    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) 
    {
        platform.Run();
        graphics.ClearScreen();
        graphics._SwapBuffers();
    }
    return 0;
}