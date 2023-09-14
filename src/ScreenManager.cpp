#include "ScreenManager.h"

ScreenManager* ScreenManager::p_instance = nullptr;

ScreenManager::ScreenManager(int hSectCount, int vSectCount)
	: frameGrabber(onNewFrameWrapper), sectionsBuffer(hSectCount, vSectCount, 3)
{
	this->hSectCount = hSectCount;
	this->vSectCount = vSectCount;

	width = frameGrabber.getWidth();
	height = frameGrabber.getHeight();

	hSectWidth = width / hSectCount;
	vSectWidth = height / vSectCount;

	sectDepth = height / 4;
}

FrameSections* ScreenManager::getCurrentSections()
{
	// wait for FrameSections::flush
	sectionsBuffer.waitForFlush();
	return sectionsBuffer.smoothSections;
}

void ScreenManager::setBrightness(float brightness)
{
	this->brightness = brightness < 0 || brightness > 1 ? 0.5 : brightness;
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

		getLeftPixelLine(imgsrc, section->left);
		getRightPixelLine(imgsrc, section->right);

		smoothFrames(section, sectionsBuffer.getPreviousSection(), sectionsBuffer.smoothSections);

		adjustBrightness(sectionsBuffer.smoothSections);

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
		int dst;
		for (int row = firstRow; (dst = abs(row - firstRow)) < sectDepth; row++)
		{
			const int src_index = row * width + col;
			const int sect = col / hSectWidth;
			const float dst_coef = (1.0 - (float)dst / sectDepth);

			sections[sect].R += (src[src_index].R * dst_coef);
			sections[sect].G += (src[src_index].G * dst_coef);
			sections[sect].B += (src[src_index].B * dst_coef);
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

void ScreenManager::getLeftPixelLine(const COM::ImageData src, Pixel* left)
{
	getVerticalPixelLine(src, left, 0);
}

void ScreenManager::getRightPixelLine(const COM::ImageData src, Pixel* right)
{
	getVerticalPixelLine(src, right, width - sectDepth - 1);
}

void ScreenManager::getVerticalPixelLine(const COM::ImageData src, Pixel* buffer, int firstCol)
{
	Pixel32 sections[256] = { 0 };

	for (int _row = 0; _row < height; _row++)
	{
		int dst;
		for (int col = firstCol; (dst = abs(col - firstCol)) < sectDepth; col++)
		{
			const int src_index = _row * width + col;
			const int sect = _row / vSectWidth;
			const float dst_coef = (1.0 - (float)dst / sectDepth);

			sections[sect].R += src[src_index].R * dst_coef;
			sections[sect].G += src[src_index].G * dst_coef;
			sections[sect].B += src[src_index].B * dst_coef;
		}
	}

	const int sectPixelsCount = vSectWidth * sectDepth;

	for (int i = 0; i < vSectCount; i++)
	{
		buffer[i].R = sections[i].R / sectPixelsCount;
		buffer[i].G = sections[i].G / sectPixelsCount;
		buffer[i].B = sections[i].B / sectPixelsCount;
	}
}

void ScreenManager::adjustBrightness(const FrameSections* section)
{
	adjustLineBrightness(section->top, hSectCount);
	adjustLineBrightness(section->bottom, hSectCount);

	adjustLineBrightness(section->left, vSectCount);
	adjustLineBrightness(section->right, vSectCount);
}

void ScreenManager::adjustLineBrightness(Pixel* buffer, uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		buffer[i].R = max(minBrightness, (int16_t)(buffer[i].R * this->brightness) + REG_CORRECTION);
		buffer[i].G = max(minBrightness, (int16_t)(buffer[i].G * this->brightness) + GREEN_CORRECTION);
		buffer[i].B = max(minBrightness, (int16_t)(buffer[i].B * this->brightness) + BLUE_CORRECTION);
	}
}

void ScreenManager::smoothFrames(const FrameSections* currentSections, const FrameSections* previousSections, const FrameSections* destSections)
{
	smoothLine(currentSections->top, previousSections->top, destSections->top, hSectCount);
	smoothLine(currentSections->bottom, previousSections->bottom, destSections->bottom, hSectCount);

	smoothLine(currentSections->left, previousSections->left, destSections->left, vSectCount);
	smoothLine(currentSections->right, previousSections->right, destSections->right, vSectCount);
}

void ScreenManager::smoothLine(Pixel* currentBuffer, const Pixel* previousBuffer, Pixel* destBuffer, uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		destBuffer[i].R = (currentBuffer[i].R + previousBuffer[i].R) / 2;
		destBuffer[i].G = (currentBuffer[i].G + previousBuffer[i].G) / 2;
		destBuffer[i].B = (currentBuffer[i].B + previousBuffer[i].B) / 2;
	}
}