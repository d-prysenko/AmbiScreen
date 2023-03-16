#include <iostream>
#include <chrono>
#include <ScreenCapture.h>

#include "utils/Image.hpp"

#define TJE_IMPLEMENTATION
#include "utils/tiny_jpeg.h"


int main()
{
    std::atomic<int> counter;
    
    std::cout << "Hello CMake." << std::endl;

    std::srand(std::time(nullptr));
    std::cout << "Starting Capture Demo/Test" << std::endl;
    std::cout << "Testing captured monitor bounds check" << std::endl;

    std::cout << "Checking for Permission to capture the screen" << std::endl;
    if (SL::Screen_Capture::IsScreenCaptureEnabled()) {
        std::cout << "Application Allowed to Capture the screen!" << std::endl;
    }
    else if (SL::Screen_Capture::CanRequestScreenCapture()) {
        std::cout << "Application Not Allowed to Capture the screen. Waiting for permission " << std::endl;
        while (!SL::Screen_Capture::IsScreenCaptureEnabled()) {
            SL::Screen_Capture::RequestScreenCapture();
            std::cout << " . ";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    else {
        std::cout << "Cannot Capture the screen due to permission issues. Exiting" << std::endl;
        return 0;
    }

    auto goodmonitors = SL::Screen_Capture::GetMonitors();
    for (auto& m : goodmonitors) {
        std::cout << m << std::endl;
        assert(SL::Screen_Capture::isMonitorInsideBounds(goodmonitors, m));
    }
    auto badmonitors = SL::Screen_Capture::GetMonitors();

    std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
    uint32_t frames = 0;

    //Setup Screen Capture for all monitors
    auto framgrabber = SL::Screen_Capture::CreateCaptureConfiguration([]() {
        return SL::Screen_Capture::GetMonitors();
    })->onNewFrame([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Monitor& monitor) {
        frames++;

        counter.fetch_add(1);

        auto s = std::to_string(counter) + std::string("MONITORDIF_") + std::string(".jpg");
        auto size = Width(img) * Height(img) * sizeof(SL::Screen_Capture::ImageBGRA);
        auto imgbuffer(std::make_unique<unsigned char[]>(size));
        Image::ExtractAndConvertToRGBA(img, imgbuffer.get(), size);
        tje_encode_to_file(s.c_str(), Width(img), Height(img), 4, (const unsigned char*)imgbuffer.get());

        //printf("Width: %d Height: %d\n", Width(img), Height(img));

        if (frames % 40 == 0) {

            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            printf("fps: %f\n", 40.0f / std::chrono::duration_cast<std::chrono::duration<double>>(now - last).count());


            last = now;
        }
    })->start_capturing();

    framgrabber->setFrameChangeInterval(std::chrono::milliseconds(70));//100 ms
    framgrabber->setMouseChangeInterval(std::chrono::milliseconds(70));//100 ms

    std::this_thread::sleep_for(std::chrono::seconds(600));

    // Create manager

    // Set leds count

    // Start capture

    // while(screenManager.isCapturing())
    // {
    //     sections = screenManager.getSections();
    //     usbManager.send(serialize(sections));
    // }



	return 0;
}
