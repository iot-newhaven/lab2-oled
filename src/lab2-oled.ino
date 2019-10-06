/*
 * Project lab2-sensor-oled
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "SparkFunMicroOLED.h" // Include MicroOLED library
#include "math.h"
#include "adxl362.h"

#define BOARD_SYSTEM_LED D7

#define SENSOR_SAMPLE_RATE_MS (100)

#define SENSOR_DISPLAY_UPDATE_MS (5000)

ADXL362 sensor;

int16_t XValue, YValue, ZValue, Temperature;

float tempAvgCelsius = 0;

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

void sensorTempGetRawVal(void)
{
    // read all three axis in burst to ensure all measurements correspond to same sample time
    sensor.readXYZTData(XValue, YValue, ZValue, Temperature);
}

float sensorTempGetCelsius(void)
{
    unsigned char rawTempData[2] = {0, 0};
    short signedTemp = 0;
    float tempCelsius = 0;

    rawTempData[0] = (char)Temperature;

    rawTempData[1] = (char)(Temperature >> 8);

    signedTemp = (short)(rawTempData[1] << 8) + rawTempData[0];

    tempCelsius = (float)signedTemp * 0.065;

    return tempCelsius;
}

// Reads sample from the sensor
void sensorTempGetSample(void)
{
    // Get raw data values from sensor
    sensorTempGetRawVal();

    // Convert raw value to celsius
    tempAvgCelsius = sensorTempGetCelsius();
}

void sensorTempDisplay(float tempCelsius)
{
    static char messageBuffer[255];

    snprintf(messageBuffer, sizeof(messageBuffer), "Temperature %f C\n", tempCelsius);

    Serial.print(messageBuffer);
}

void sensorTempDisplayAverage(void)
{
    sensorTempDisplay(tempAvgCelsius);
}

Timer timerSensorTempDisplay(SENSOR_DISPLAY_UPDATE_MS, sensorTempDisplayAverage);

Timer timerSensorTempSample(SENSOR_SAMPLE_RATE_MS, sensorTempGetSample);

void sensorTempInit(void)
{
    sensor.begin(SS);      // Setup SPI protocol, issue device soft reset
    sensor.beginMeasure(); // Switch ADXL362 to measure mode

    // Start Temperature sample timer
    timerSensorTempSample.start();

    // Start Temperature display timer
    timerSensorTempDisplay.start();

}

void systemHeartbeat(void)
{
    static bool firstTime = true;
    static bool ledState = true;

    if (firstTime)
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
    Serial.println("Lab2: OLED sensor (task 2b)");

    sensorTempInit();

    displayInit();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // The core of your code will likely live here.
    systemHeartbeat();
}