#pragma once
#include <vector>
#include <condition_variable>

#include "utils/Image.hpp"

struct FrameSections : public Serializable
{
	std::mutex mtx;

	uint8_t width;
	uint8_t height;

	Pixel* top = nullptr;
	Pixel* bottom = nullptr;
	Pixel* left = nullptr;
	Pixel* right = nullptr;

	FrameSections(uint8_t width, uint8_t height)
		: top(new Pixel[width]), bottom(new Pixel[width]), left(new Pixel[height]), right(new Pixel[height]),
		width(width), height(height)
	{}

	~FrameSections()
	{
		delete[] top, bottom, left, right;
	}

	void clean()
	{
		memset(top, 0, width * sizeof(Pixel));
		memset(bottom, 0, width * sizeof(Pixel));
		memset(left, 0, height * sizeof(Pixel));
		memset(right, 0, height * sizeof(Pixel));
	}

	std::vector<uint8_t> serialize()
	{
		std::vector<uint8_t> res;
		
		// signature
		res.push_back(0xF0);
		res.push_back(0x0D);
		res.push_back(0xBA);
		res.push_back(0xBE);

		// size
		res.push_back(width);
		res.push_back(height);

		// data
		for (int i = 0; i < width; i++)
		{
			res.push_back(top[i].R);
			res.push_back(top[i].G);
			res.push_back(top[i].B);
		}

		for (int i = height - 1; i >= 0 ; i--)
		{
			res.push_back(right[i].R);
			res.push_back(right[i].G);
			res.push_back(right[i].B);
		}

		for (int i = width - 1; i >= 0; i--)
		{
			res.push_back(bottom[i].R);
			res.push_back(bottom[i].G);
			res.push_back(bottom[i].B);
		}

		for (int i = 0; i < height; i++)
		{
			res.push_back(left[i].R);
			res.push_back(left[i].G);
			res.push_back(left[i].B);
		}

		return res;
	}
};

struct FrameSectionsBuffer
{
	std::condition_variable cv;
	std::mutex mtx;

	bool flushed = false;

	uint8_t bufferDepth = 2;

	FrameSections** sections = nullptr;

	uint8_t currentSection = 0;

	uint8_t width = 0;
	uint8_t height = 0;

	FrameSectionsBuffer()
		: sections(new FrameSections* [bufferDepth])
	{}

	FrameSectionsBuffer(uint8_t width, uint8_t height, uint8_t bufferDepth = 2)
		: sections(new FrameSections* [bufferDepth]), width(width), height(height), bufferDepth(bufferDepth)
	{
		for (int i = 0; i < bufferDepth; i++)
		{
			sections[i] = new FrameSections(width, height);
		}
	}

	FrameSections* getCurrentSection()
	{
		return sections[currentSection];
	}

	FrameSections* getNextSection()
	{
		return sections[(currentSection + 1) % bufferDepth];
	}

	FrameSectionsBuffer& cleanNextSection()
	{
		FrameSections* section = getNextSection();

		section->clean();

		return *this;
	}

	void flush()
	{
		currentSection = ++currentSection % bufferDepth;
		flushed = true;
		cv.notify_all();
	}

	void waitForFlush()
	{
		std::unique_lock<std::mutex> lk(mtx);
		cv.wait(lk, [&] {return flushed == true; });
		flushed = false;
	}
};