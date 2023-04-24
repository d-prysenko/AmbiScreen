#include "ScreenManager.h"

ScreenManager* ScreenManager::p_instance = nullptr;

ScreenManager::ScreenManager(int hSectCount, int vSectCount)
	: frameGrabber(onNewFrameWrapper), sectionsBuffer(hSectCount, vSectCount)
{
	this->hSectCount = hSectCount;
	this->vSectCount = vSectCount;

	width = frameGrabber.getWidth();
	height = frameGrabber.getHeight();

	hSectWidth = width / hSectCount;
	vSectWidth = height / vSectCount;

	sectDepth = hSectWidth;
}

FrameSections* ScreenManager::getCurrentSections()
{
	// wait for FrameSections::flush
	sectionsBuffer.waitForFlush();
	return sectionsBuffer.getCurrentSection();
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
		FrameSections* section = sectionsBuffer.getNextSection();

		const std::lock_guard<std::mutex> lock(section->mtx);

		sectionsBuffer.cleanNextSection();

		getTopPixelLine(imgsrc, section->top);
		getBottomPixelLine(imgsrc, section->bottom);

		sectionsBuffer.flush();
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
	Pixel32 sections[256] = { 0 };

	for (int col = 0; col < width; col++)
	{
		for (int row = firstRow; row < sectDepth; row++)
		{
			const int src_index = row * width + col;
			const int sect = col / hSectWidth;

			sections[sect].R += src[src_index].R;
			sections[sect].G += src[src_index].G;
			sections[sect].B += src[src_index].B;
		}
	}

	const int sectPixelsCount = hSectWidth * sectDepth;

	for (int i = 0; i < hSectCount; i++)
	{
		buffer[i].R = sections[i].R / sectPixelsCount;
		buffer[i].G = sections[i].G / sectPixelsCount;
		buffer[i].B = sections[i].B / sectPixelsCount;
	}
}