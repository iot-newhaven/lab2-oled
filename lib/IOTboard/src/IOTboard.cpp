#include "IOTboard.h"

static void heartbeat(void);

// MicroOLED Object Declaration
MicroOLED oled(MODE_I2C, D8, 1);

// Particle soft timer API
// Create heartbeat heartbeat_tmr
Timer heartbeat_tmr(BOARD_HEARTBEAT_MS, heartbeat);

// Sensors
TMP102 temp_sensor;

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

    delay(100);
    
    // Start TM102 Sensor
    temp_sensor.begin();

    delay(100);

    // set the Conversion Rate (how quickly the sensor gets a new reading)
    //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
    temp_sensor.setConversionRate(2);

    //set Extended Mode.
    //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
    temp_sensor.setExtendedMode(0);

    // start heartbeat timer
    heartbeat_tmr.start();

    // Start serial port
    Serial.begin(115200); // open serial over USB
}

void IOTboard::printToDisplay(const char* text) {
    
    oled.setCursor(0, 0); // Set cursor to top-left
    oled.clear(PAGE);     // Clear the screen
    oled.print(text);

    oled.display();

    delayMicroseconds(100);
}

float IOTboard::getTempF() {
    // The core of your code will likely live here.
    float temperature;

    // Turn sensor on to start temperature measurement.
    // Current consumtion typically ~10uA.
    temp_sensor.wakeup();

    // read temperature data
    temperature = temp_sensor.readTempF();
    //temperature = sensor.readTempC();

    // Place sensor in sleep mode to save power.
    // Current consumtion typically <0.5uA.
    temp_sensor.sleep();

    delayMicroseconds(100);

    return (temperature);
}

void IOTboard::serialLog(const char *fmt, ...)
{
    va_list args;
    
    va_start(args, fmt);

    memset(serial_out_buffer, 0, sizeof(serial_out_buffer));

    vsnprintf(serial_out_buffer, sizeof(serial_out_buffer), fmt, args);

    va_end(args);

    Serial.printlnf(serial_out_buffer);
}
