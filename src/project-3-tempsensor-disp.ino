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

    temperature = iot_kit.getTempF();

    iot_kit.serialLog("Temperature: %f", temperature);

    memset(temperature_str, 0, sizeof(temperature_str));
    snprintf(temperature_str, sizeof(temperature_str), "%.2f", temperature);

    iot_kit.printToDisplay(temperature_str);

    delay(1000);
}