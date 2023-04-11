#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
	checkPermissions();
	getCaptureConfig();
}

ScreenManager::ScreenManager(int hSectCount, int vSectCount)
	: ScreenManager()
{
	this->hSectCount = hSectCount;
	this->vSectCount = vSectCount;

	std::vector<Monitor> monitors = getMonitors();

	// FIXME
	Monitor monitor = monitors.back();

	width = monitor.Width;
	height = monitor.Height;

	hSectWidth = width / hSectCount;
	vSectWidth = height / vSectCount;

	sectDepth = hSectWidth;

	sections.top = new Pixel[hSectCount];
	sections.bottom = new Pixel[hSectCount];
}

void ScreenManager::startCapture()
{
	auto framgrabber = captureConfig->start_capturing();

	framgrabber->setFrameChangeInterval(std::chrono::milliseconds(70));
	framgrabber->setMouseChangeInterval(std::chrono::milliseconds(70));
}

FrameSectionsBuffer ScreenManager::getSections()
{
	return FrameSectionsBuffer();
}

void ScreenManager::checkPermissions()
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

std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> ScreenManager::getCaptureConfig()
{
	if (captureConfig == nullptr) {
		captureConfig = SL::Screen_Capture::CreateCaptureConfiguration([&]() { return getMonitors(); })
			->onNewFrame([&](const Image& img, const Monitor& monitor) { onNewFrame(img, monitor); });
	}

	return captureConfig;
}

std::vector<Monitor> ScreenManager::getMonitors()
{
	return SL::Screen_Capture::GetMonitors();
}

void ScreenManager::onNewFrame(const Image& img, const Monitor& monitor)
{
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
	std::atomic<int> counter;
	uint32_t frames = 0;
	frames++;

	counter.fetch_add(1);



	const SL::Screen_Capture::ImageBGRA* imgsrc = StartSrc(img);

	memset(sections.top, 0, hSectCount * sizeof(Pixel));
	memset(sections.bottom, 0, hSectCount * sizeof(Pixel));

	getTopPixelLine(imgsrc, sections.top);
	getBottomPixelLine(imgsrc, sections.bottom);


	auto s = std::to_string(counter) + std::string("MONITORDIF_") + std::string(".jpg");
	auto size = Width(img) * Height(img) * sizeof(SL::Screen_Capture::ImageBGRA);
	auto imgbuffer(std::make_unique<unsigned char[]>(size));


	//printf("Width: %d Height: %d\n", Width(img), Height(img));

	if (frames % 40 == 0) {

		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		printf("fps: %f\n", 40.0f / std::chrono::duration_cast<std::chrono::duration<double>>(now - last).count());


		last = now;
	}
}

void ScreenManager::getTopPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* top)
{
	getHorizontalPixelLine(src, top, 0);
}

void ScreenManager::getBottomPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* top)
{
	getHorizontalPixelLine(src, top, height - sectDepth - 1);
}

void ScreenManager::getHorizontalPixelLine(const SL::Screen_Capture::ImageBGRA* src, Pixel* buffer, int firstRow)
{
	for (int col = 0; col < width; col++)
	{
		for (int row = firstRow; row < sectDepth; row++)
		{
			const int index = row * width + col;

			buffer[col / hSectWidth].R += src[index].R;
			buffer[col / hSectWidth].G += src[index].G;
			buffer[col / hSectWidth].B += src[index].B;
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