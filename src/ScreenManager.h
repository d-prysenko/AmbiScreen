#pragma once
#include <cstdint>
#include <chrono>
#include <exception>

#include "utils/Image.hpp"
#include "utils/FrameGrabber.hpp"

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
	Pixel* top, * bottom, * left, * right;
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

	FrameSectionsBuffer sections;

	//CornerResolveMethod cornerResolveMethod;


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

	FrameSectionsBuffer getSections();

	

private:
	static void onNewFrameWrapper(const COM::Image& img);
	void onNewFrame(const COM::Image& img);

	void getTopPixelLine(const COM::ImageData src, Pixel* top);
	void getBottomPixelLine(const COM::ImageData src, Pixel* top);
	void getHorizontalPixelLine(const COM::ImageData src, Pixel* buffer, int firstRow);
};

