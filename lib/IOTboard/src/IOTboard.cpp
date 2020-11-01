#include "IOTboard.h"

static void heartbeat(void);

// MicroOLED Object Declaration
MicroOLED oled(MODE_I2C, D8, 1);

// Create heartbeat heartbeat_tmr
Timer heartbeat_tmr(BOARD_HEARTBEAT_MS, heartbeat);

static void heartbeat(void)
{
    static bool ledState = true;

    if (ledState)
    {
        digitalWrite(BOARD_SYSTEM_LED, LOW);

        ledState = false;
    }
    else
    {
        digitalWrite(BOARD_SYSTEM_LED, HIGH);

        ledState = true;
    }
}

// Class constructor
IOTboard::IOTboard() {

}

void IOTboard::start() {
    // Setup outputs
    pinMode(BOARD_SYSTEM_LED, OUTPUT);
    digitalWrite(BOARD_SYSTEM_LED, HIGH);

    // Start OLED display
    oled.begin();         // Initialize the OLED
    oled.setFontType(1);  // Set font to type 0
    oled.setCursor(0, 0); // Set cursor to top-left
    oled.clear(PAGE);     // Clear the screen

    heartbeat_tmr.start();
}

void IOTboard::printToDisplay(const char* text) {
    oled.print(text);
    oled.display();
}

