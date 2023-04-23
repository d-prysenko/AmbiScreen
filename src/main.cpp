#include <iostream>

#include "ScreenManager.h"
#include "Serial.hpp"

const int VERTICAL_LEDS_COUNT = 30;
const int HORIZONTAL_LEDS_COUNT = 20;

#define BUFSIZE 1024



int main()
{

    // Create manager

    // Set leds count

    // Start capture

    // while(screenManager.isCapturing())
    // {
    //     sections = screenManager.getSections();
    //     usbManager.send(serialize(sections));
    // }


	Serial serial("COM1");

    ScreenManager* screenManager = ScreenManager::getInstance(HORIZONTAL_LEDS_COUNT, VERTICAL_LEDS_COUNT);
    
    screenManager->startCapture();

	return 0;
}
