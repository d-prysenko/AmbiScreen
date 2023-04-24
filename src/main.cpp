#include <iostream>

#include "ScreenManager.h"
#include "Serial.hpp"

const int HORIZONTAL_LEDS_COUNT = 32;
const int VERTICAL_LEDS_COUNT = 19;

int main()
{
	Serial serial("COM3");

    ScreenManager* screenManager = ScreenManager::getInstance(HORIZONTAL_LEDS_COUNT, VERTICAL_LEDS_COUNT);
    
    screenManager->startCapture();

    // wait for screen capture init
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    FrameSections* sections;

    while (sections = screenManager->getCurrentSections())
    {
        const std::lock_guard<std::mutex> lock(sections->mtx);

        serial.write(sections);
    }

	return 0;
}
