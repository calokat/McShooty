namespace PE
{
	enum Platform { Win32, Web, Android, Linux };
	enum GraphicsAPI { OpenGL, DirectX11 };
	enum XrPlatform { NoXR, OpenXR };
}
struct Options
{
	PE::Platform platform;
	PE::GraphicsAPI graphicsAPI;
	PE::XrPlatform xr = PE::XrPlatform::NoXR;
};
