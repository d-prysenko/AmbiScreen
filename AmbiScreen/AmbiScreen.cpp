#include <iostream>
#include <ScreenCapture.h>

inline std::ostream& operator<<(std::ostream& os, const SL::Screen_Capture::ImageRect& p)
{
    return os << "left=" << p.left << " top=" << p.top << " right=" << p.right << " bottom=" << p.bottom;
}
inline std::ostream& operator<<(std::ostream& os, const SL::Screen_Capture::Monitor& p)
{
    return os << "Id=" << p.Id << " Index=" << p.Index << " Height=" << p.Height << " Width=" << p.Width << " OffsetX=" << p.OffsetX
        << " OffsetY=" << p.OffsetY << " Name=" << p.Name;
}

int main()
{
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


	return 0;
}
