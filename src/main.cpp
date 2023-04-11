#include <iostream>

#include "ScreenManager.h"

//const int VERTICAL_LEDS_COUNT = 30;
const int HORIZONTAL_LEDS_COUNT = 20;
#define VERTICAL_LEDS_COUNT 30


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

    ScreenManager screenManager = ScreenManager(HORIZONTAL_LEDS_COUNT, VERTICAL_LEDS_COUNT);
    
    screenManager.startCapture();

	return 0;
}
