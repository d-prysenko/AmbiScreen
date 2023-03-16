#pragma once
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

class Image
{
public:
	static void ExtractAndConvertToRGBA(const SL::Screen_Capture::Image& img, unsigned char* dst, size_t dst_size)
    {
        assert(dst_size >= static_cast<size_t>(SL::Screen_Capture::Width(img) * SL::Screen_Capture::Height(img) * sizeof(SL::Screen_Capture::ImageBGRA)));
        auto imgsrc = StartSrc(img);
        auto imgdist = dst;
        for (auto h = 0; h < Height(img); h++) {
            auto startimgsrc = imgsrc;
            for (auto w = 0; w < Width(img); w++) {
                *imgdist++ = imgsrc->R;
                *imgdist++ = imgsrc->G;
                *imgdist++ = imgsrc->B;
                *imgdist++ = 0;
                imgsrc++;
            }
            imgsrc = SL::Screen_Capture::GotoNextRow(img, startimgsrc);
        }
    }
};