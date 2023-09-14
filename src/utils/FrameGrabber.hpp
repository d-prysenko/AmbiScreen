#include <ScreenCapture.h>
#include "utils/Image.hpp"

using SL::Screen_Capture::ICaptureConfiguration;
using SL::Screen_Capture::ScreenCaptureCallback;
using SL::Screen_Capture::Monitor;

typedef void (*frameCallback)(const COM::Image&);

class FrameGrabber
{
private:
	frameCallback newFrameCallback;

public:

	FrameGrabber(frameCallback newFrameCallback2)
		: newFrameCallback(newFrameCallback2)
	{
		checkPermissions();
		getCaptureConfig();
	}

private:

	std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> captureConfig = nullptr;

	void checkPermissions()
	{
		std::cout << "Checking for Permission to capture the screen" << std::endl;

		if (SL::Screen_Capture::IsScreenCaptureEnabled())
		{
			std::cout << "Application Allowed to Capture the screen!" << std::endl;
		}
		else if (SL::Screen_Capture::CanRequestScreenCapture())
		{
			std::cout << "Application Not Allowed to Capture the screen. Waiting for permission " << std::endl;

			while (!SL::Screen_Capture::IsScreenCaptureEnabled())
			{
				SL::Screen_Capture::RequestScreenCapture();
				std::cout << " . ";
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		else
		{
			std::cerr << "Cannot Capture the screen due to permission issues. Exiting" << std::endl;
			throw std::exception("Cannot Capture the screen due to permission issues. Exiting");
		}
	}

	std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> getCaptureConfig()
	{
		if (captureConfig == nullptr) {
			captureConfig = SL::Screen_Capture::CreateCaptureConfiguration([&]() { return SL::Screen_Capture::GetMonitors(); })
				->onNewFrame([&](const SL::Screen_Capture::Image& img, const Monitor& monitor) { onNewFrame(img, monitor); });
		}

		return captureConfig;
	}

	void onNewFrame(const SL::Screen_Capture::Image& img, const Monitor& monitor)
	{
		newFrameCallback(COM::Image(img));
	}

public:

	void startCapture()
	{
		auto framgrabber = captureConfig->start_capturing();

		framgrabber->setFrameChangeInterval(std::chrono::milliseconds(20));
		framgrabber->setMouseChangeInterval(std::chrono::milliseconds(20));
	}

	int getWidth()
	{
		std::vector<Monitor> monitors = SL::Screen_Capture::GetMonitors();
		Monitor monitor = monitors.back();
		return monitor.Width;
	}

	int getHeight()
	{
		std::vector<Monitor> monitors = SL::Screen_Capture::GetMonitors();
		Monitor monitor = monitors.back();
		return monitor.Height;
	}

};