/*
 * Project project-3-tempsensor-disp
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "IOTboard.h"

IOTboard iot_kit;

// setup() runs once, when the device is first turned on.
void setup()
{
    Serial.begin(115200);
    Serial.println("Lab2: OLED sensor (task 2a)");

    iot_kit.start();

    iot_kit.printToDisplay("Hello");
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    
}