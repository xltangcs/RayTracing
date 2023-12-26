#pragma once

#ifdef TF_PLATFORM_WINDOWS

extern Toffee::Application* Toffee::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Toffee {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			Toffee::Application* app = Toffee::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}

}

#ifdef WL_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Toffee::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Toffee::Main(argc, argv);
}

#endif // WL_DIST

#endif // WL_PLATFORM_WINDOWS
