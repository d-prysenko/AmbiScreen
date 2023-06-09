#pragma once
#include <iostream>
#include <ScreenCapture.h>

#include "Serializable.hpp"

inline std::ostream& operator<<(std::ostream& os, const SL::Screen_Capture::ImageRect& p)
{
    return os << "left=" << p.left << " top=" << p.top << " right=" << p.right << " bottom=" << p.bottom;
}

inline std::ostream& operator<<(std::ostream& os, const SL::Screen_Capture::Monitor& p)
{
    return os << "Id=" << p.Id << " Index=" << p.Index << " Height=" << p.Height << " Width=" << p.Width << " OffsetX=" << p.OffsetX
        << " OffsetY=" << p.OffsetY << " Name=" << p.Name;
}

struct Pixel
{
	uint8_t R, G, B;
};

struct Pixel32
{
	uint32_t R, G, B;
};

namespace COM
{

    typedef const SL::Screen_Capture::ImageBGRA* ImageData;

    struct Image
    {
        const ImageData data;
        
        Image(const SL::Screen_Capture::Image& img)
            : data(img.Data)
        { }

        ImageData getData() const
        {
            return data;
        }
    };
}

