#include "ScreenManager.h"

ScreenManager* ScreenManager::p_instance = nullptr;

ScreenManager::ScreenManager(int hSectCount, int vSectCount)
	: frameGrabber(onNewFrameWrapper)
{
	this->hSectCount = hSectCount;
	this->vSectCount = vSectCount;

	width = frameGrabber.getWidth();
	height = frameGrabber.getHeight();

	hSectWidth = width / hSectCount;
	vSectWidth = height / vSectCount;

	sectDepth = hSectWidth;

	sections.top = new Pixel[hSectCount];
	sections.bottom = new Pixel[hSectCount];
	sections.left = new Pixel[vSectCount];
	sections.right = new Pixel[vSectCount];
}


FrameSectionsBuffer ScreenManager::getSections()
{
	return sections;
}

void ScreenManager::startCapture()
{
	frameGrabber.startCapture();
}

void ScreenManager::onNewFrameWrapper(const COM::Image& img)
{
	if (!p_instance)
		return;

	static_cast<ScreenManager*>(p_instance)->onNewFrame(img);
}

void ScreenManager::onNewFrame(const COM::Image& img)
{
	COM::ImageData imgsrc = img.getData();

	{
		// lock here?

		memset(sections.top, 0, hSectCount * sizeof(Pixel));
		memset(sections.bottom, 0, hSectCount * sizeof(Pixel));

		getTopPixelLine(imgsrc, sections.top);
		getBottomPixelLine(imgsrc, sections.bottom);
	}
}

void ScreenManager::getTopPixelLine(const COM::ImageData src, Pixel* top)
{
	getHorizontalPixelLine(src, top, 0);
}

void ScreenManager::getBottomPixelLine(const COM::ImageData src, Pixel* bottom)
{
	getHorizontalPixelLine(src, bottom, height - sectDepth - 1);
}

void ScreenManager::getHorizontalPixelLine(const COM::ImageData src, Pixel* buffer, int firstRow)
{
	for (int col = 0; col < width; col++)
	{
		for (int row = firstRow; row < sectDepth; row++)
		{
			const int src_index = row * width + col;
			const int sect = col / hSectWidth;

			buffer[sect].R += src[src_index].R;
			buffer[sect].G += src[src_index].G;
			buffer[sect].B += src[src_index].B;
		}
	}

	const int sectPixelsCount = hSectWidth * sectDepth;

	for (int i = 0; i < hSectCount; i++)
	{
		buffer[i].R /= sectPixelsCount;
		buffer[i].G /= sectPixelsCount;
		buffer[i].B /= sectPixelsCount;
	}
}