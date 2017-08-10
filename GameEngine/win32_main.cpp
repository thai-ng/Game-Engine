#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <mutex>

#include "Window.hpp"
#include "Frame.hpp"


using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

constexpr static auto FPS = 30;
constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(static_cast<int>(1000.0 / FPS));
static bool Running = true;

struct GameState
{
};


int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE,
		LPSTR,
		int)
{
	GE::Window window(Instance);

	auto buffer = window.FrameBuffer();
	auto deviceContext = window.DeviceContext();
	auto windowHandle = window.Handle();
	auto width = window.Width();
	auto height = window.Height();

	std::vector<long long> frameTime(width);
	//GameState game;

	auto lastRenderTime = std::chrono::high_resolution_clock::now();

	while (Running)
	{
		MSG msg;
		while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_KEYDOWN:
			{
				OutputDebugString("KeyDown!\n");
			} break;
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} break;
			}
		}

		// Do game update

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRenderTime);
		if (elapsedTime >= MillisecondsPerFrame)
		{
			std::stringstream str;
			str << "Elapsed time: " << elapsedTime.count() << "ms Target time: " << MillisecondsPerFrame.count() << "ms\n";
			OutputDebugString(str.str().c_str());

			GE::Frame frame(buffer, deviceContext, width, height, frameTime);
			// Draw

			buffer.FillFrame();
			buffer.DrawTargetFrameTime(33);
			buffer.DrawFrameTime(frameTime);
			lastRenderTime = std::chrono::high_resolution_clock::now();
		}
	}

	return 0;
}