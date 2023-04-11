#pragma once
#include <cstdint>
#include <chrono>
#include <exception>

#include <mutex>

#include <ScreenCapture.h>
#include "utils/Image.hpp"

using SL::Screen_Capture::ICaptureConfiguration;
using SL::Screen_Capture::ScreenCaptureCallback;
using SL::Screen_Capture::Monitor;
using SL::Screen_Capture::Image;

enum CornerResolveMethod : uint8_t
{
	Split,
	HalfIntersection,
	Intersection,
};

struct Pixel
{
	uint8_t R, G, B;
};

struct FrameSectionsBuffer
{
	Pixel *top, *bottom, *left, *right;
};

class ScreenManager
{
public:
	

private:

	int width;
	int height;

	int hSectCount;
	int vSectCount;

	int hSectWidth;
	int vSectWidth;
	int sectDepth;

	FrameSectionsBuffer sections;

	CornerResolveMethod cornerResolveMethod;

	std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> captureConfig = nullptr;

	std::mutex buffer;


public:
	ScreenManager();

	/// <param name="hSectCount">Vertical sections count</param>
	/// <param name="vSectCount">Horizontal Sections count</param>
	ScreenManager(int hSectCount, int vSectCount);

	void startCapture();

	ScreenManager setVerticalSectionsCount(int);
	ScreenManager setHorizontalSectionsCount(int);
	ScreenManager setCornerResolveMethod(CornerResolveMethod);
	
	bool isCapturing();

	// std::condition_variable::wait
	FrameSectionsBuffer getSections();

private:
	void checkPermissions();

	std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> getCaptureConfig();
	std::vector<Monitor> getMonitors();
	void onNewFrame(const Image& img, const Monitor& monitor);

	void getTopPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* top);
	void getBottomPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* top);
	void getHorizontalPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* buffer, int firstRow);
};