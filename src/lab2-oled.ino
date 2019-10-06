/*
 * Project lab2-sensor-oled
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "SparkFunMicroOLED.h" // Include MicroOLED library
#include "math.h"

#define BOARD_SYSTEM_LED    D7


//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(MODE_I2C, D8, 1); // I2C declaration

void displayInit(void)
{
    oled.begin();         // Initialize the OLED
    oled.clear(PAGE);     // Clear the screen
    oled.setFontType(1);  // Set font to type 0
    oled.setCursor(0, 0); // Set cursor to top-left

    oled.print("Hello!");
    oled.display();
}

void systemHeartbeat(void)
{
    static bool firstTime = true;
    static bool ledState = true;

    if(firstTime)
    {
        pinMode(BOARD_SYSTEM_LED, OUTPUT);

        firstTime = false;

        digitalWrite(BOARD_SYSTEM_LED, HIGH);
    }
    else
    {
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

    delay(1000); // Delay 1000 ms
}

// setup() runs once, when the device is first turned on.
void setup()
{
    // Put initialization like pinMode and begin functions here.
    delay(100);

    Serial.begin(115200);
    Serial.println("Lab2: OLED sensor (task 2a)");

    displayInit();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // The core of your code will likely live here.
    systemHeartbeat();
}