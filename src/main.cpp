#include <iostream>

#include "ScreenManager.h"
#include "Serial.hpp"

const int HORIZONTAL_LEDS_COUNT = 31;
const int VERTICAL_LEDS_COUNT = 18;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Wrong arguments, need port name, argc: %d\n", argc);
        return -1;
    }

	Serial serial(argv[1]);

    ScreenManager* screenManager = ScreenManager::getInstance(HORIZONTAL_LEDS_COUNT, VERTICAL_LEDS_COUNT);
    
    screenManager->setBrightness(0.35);
    screenManager->startCapture();

    // wait for screen capture init
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    FrameSections* sections;

    while (sections = screenManager->getCurrentSections())
    {
        const std::lock_guard<std::mutex> lock(sections->mtx);
        //std::cout << "write" << std::endl;
        serial.write(sections);
    }

	return 0;
}
