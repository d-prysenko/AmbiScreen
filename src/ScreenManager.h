#pragma once
#include <cstdint>
#include <chrono>
#include <exception>
#include <condition_variable>

#include "utils/Image.hpp"
#include "utils/FrameGrabber.hpp"
#include "utils/FrameSection.hpp"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

enum CornerResolveMethod : uint8_t
{
	Split,
	HalfIntersection,
	Intersection,
};

class ScreenManager
{
public:
	/// <param name="hSectCount">Vertical sections count</param>
	/// <param name="vSectCount">Horizontal Sections count</param>
	ScreenManager(int hSectCount, int vSectCount);
	
private:

	static ScreenManager* p_instance;

	FrameGrabber frameGrabber;

	int width;
	int height;

	int hSectCount;
	int vSectCount;

	int hSectWidth;
	int vSectWidth;
	int sectDepth;

	FrameSectionsBuffer sectionsBuffer;

	std::condition_variable cv;
	std::mutex mutex;
	//CornerResolveMethod cornerResolveMethod;

	float brightness = 0.5;
	uint8_t minBrightness = 5;

	const int8_t REG_CORRECTION = 0;
	const int8_t GREEN_CORRECTION = 0;
	const int8_t BLUE_CORRECTION = -3;

public:
	static ScreenManager* getInstance(int hSectCount, int vSectCount)
	{
		if (!p_instance) {
			p_instance = new ScreenManager(hSectCount, vSectCount);
		}

		return p_instance;
	}

	void startCapture();

	//ScreenManager setVerticalSectionsCount(int);
	//ScreenManager setHorizontalSectionsCount(int);
	//ScreenManager setCornerResolveMethod(CornerResolveMethod);

	//bool isCapturing();

	FrameSections* getCurrentSections();

	void setBrightness(float brightness);


private:
	static void onNewFrameWrapper(const COM::Image& img);
	void onNewFrame(const COM::Image& img);

	void getTopPixelLine(const COM::ImageData src, Pixel* top);
	void getBottomPixelLine(const COM::ImageData src, Pixel* bottom);
	void getHorizontalPixelLine(const COM::ImageData src, Pixel* buffer, int firstRow);

	void getLeftPixelLine(const COM::ImageData src, Pixel* left);
	void getRightPixelLine(const COM::ImageData src, Pixel* right);
	void getVerticalPixelLine(const COM::ImageData src, Pixel* buffer, int firstCol);

	void adjustBrightness(const FrameSections* section);
	void adjustLineBrightness(Pixel* buffer, uint8_t len);

	void smoothFrames(const FrameSections* currentSections, const FrameSections* previousSections, const FrameSections* destSections);
	void smoothLine(Pixel* currentBuffer, const Pixel* previousBuffer, Pixel* destBuffer, uint8_t len);
};

