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
    iot_kit.start();

    iot_kit.printToDisplay("Hello");
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    float temperature;
    static char temperature_str[8];
    static unsigned long start_time = millis();
    char cmd;

    if (millis() - start_time >= 1000)
    {
        temperature = iot_kit.getTempF();

        iot_kit.serialLog("Temperature: %f", temperature);

        memset(temperature_str, 0, sizeof(temperature_str));
        snprintf(temperature_str, sizeof(temperature_str), "%.2f", temperature);

        iot_kit.printToDisplay(temperature_str);

        start_time = millis();
    }

    cmd = iot_kit.inputProcessor();

    if (cmd != 0)
    {
        iot_kit.serialLog("Command: %c", cmd);
    }
}