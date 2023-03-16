#pragma once
#include <cstdint>

static class FrameSectionsBuffer
{
public:
	enum CornerResolveMethod : uint8_t
	{
		Split,
		HalfIntersection,
		Intersection,
	};

private:

};

class ScreenManager
{
public:
	

private:
	int hSectCount;
	int vSectCount;
	FrameSectionsBuffer::CornerResolveMethod cornerResolveMethod;

public:
	ScreenManager();

	/// <param name="hSectCount">Vertical sections count</param>
	/// <param name="vSectCount">Horizontal Sections count</param>
	ScreenManager(int hSectCount, int vSectCount);

	ScreenManager setVerticalSectionsCount(int);
	ScreenManager setHorizontalSectionsCount(int);
	ScreenManager setCornerResolveMethod(FrameSectionsBuffer::CornerResolveMethod);
	
	bool isCapturing();

	// std::condition_variable::wait
	FrameSectionsBuffer getSections();
};