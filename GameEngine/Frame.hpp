#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <chrono>
#include <thread>
#include <sstream>

#include "Buffer.hpp"

namespace GE
{

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Frame
{
public:
	Frame(Buffer& buffer, HDC& deviceContext, int width, int height, std::vector<long long>& times) :
		m_screenBuffer(buffer),
		m_deviceContext(deviceContext),
		m_width(width),
		m_height(height),
		frameTime(times)
	{
		frameBeginTime = std::chrono::high_resolution_clock::now();
	}

	~Frame()
	{
		StretchDIBits(m_deviceContext,
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			m_screenBuffer.Data(),
			m_screenBuffer.BMapInfoPtr(),
			DIB_RGB_COLORS,
			SRCCOPY);

		auto frameEndTime = std::chrono::high_resolution_clock::now();

		EndFrame(frameBeginTime, frameEndTime);
	}

	TimePoint frameBeginTime;

private:
	inline
	void EndFrame(TimePoint beginFrame, TimePoint endFrame)
	{
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endFrame - beginFrame);

		if (frameTime.size() == frameTime.capacity())
		{
			std::rotate(frameTime.begin(), std::next(frameTime.begin()), frameTime.end());
			frameTime[frameTime.size() - 1] = elapsedTime.count();
		}
		else
		{
			frameTime.push_back(elapsedTime.count());
		}

		// if (elapsedTime > MillisecondsPerFrame)
		// {
		// }
		auto skippedFrame = elapsedTime / MillisecondsPerFrame;

		std::stringstream str;
		str << "Drawing took: " << elapsedTime.count() << "ms\n";
		OutputDebugString(str.str().c_str());
	}

	int m_width;
	int m_height;
	HDC& m_deviceContext;
	Buffer& m_screenBuffer;

	std::vector<long long>& frameTime;

	constexpr static auto FPS = 30;
	constexpr static auto MillisecondsPerFrame = std::chrono::milliseconds(static_cast<int>(1000.0 / FPS));
};

}