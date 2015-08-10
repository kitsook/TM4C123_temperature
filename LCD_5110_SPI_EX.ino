// By Clarence Ho (2015)
// TM4C123 connected to Nokia 5110 LCD display and DHT11 sensor
//
// Nokia LCD 5110 part based on sample of LCD_5110_SPI library

// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif


// Include application, user and local libraries
#include "SPI.h"
#include "LCD_5110_SPI.h"
#include "dht11.h"

// Variables
/// P._. / PB_4 = SCK (2) = Serial Clock
/// P._. / PB_7 = MOSI (2) = Serial Data

#if defined(__MSP430G2553__)
LCD_5110_SPI myScreen;
#elif defined(__LM4F120H5QR__)
LCD_5110_SPI myScreen(PA_7,    // Chip Select
                      PA_2,    // Data/Command
                      PB_5,    // Reset
                      PA_6,    // Backlight
                      PUSH2);  // Push Button 2
#endif

dht11 DHT11;
// DHT11 data pin
#define DHT11_PIN PD_7

volatile uint8_t count = 0;

// Add setup code
void setup() {
#if defined(__MSP430G2553__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#elif defined(__LM4F120H5QR__)
    //SPI.Select(2);
    SPI.setModule(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV128); // for LM4F120H5QR DIV2 = 4 MHz !
#endif
    
    myScreen.begin();
    myScreen.setContrast(0x58);
    myScreen.clear();
    myScreen.text(0, 0, "Initializing");
    
    // for DHT11 to settle
    delay(1000);

    myScreen.clear();
    Serial.begin(115200);
}

// Add loop code
void loop() {
    myScreen.clear();
    
    // read temp and humidity
    int chk = DHT11.read(DHT11_PIN);

    Serial.print("Read sensor: ");
    switch (chk) {
        case DHTLIB_OK: 
            Serial.println("OK"); 
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            Serial.println("Checksum error"); 
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            Serial.println("Time out error"); 
            break;
        default: 
            Serial.println("Unknown error"); 
            break;
    }
    
    if (chk != DHTLIB_OK) {
        myScreen.text(0, 0, "Problem reading sensor... wait...");
        delay(2000);
        return;
    }

    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);

    Serial.print("Temperature (°C): ");
    Serial.println((float)DHT11.temperature, 2);

    myScreen.setFont(0);
    myScreen.text(0, count + 0, "Temp   :");
    myScreen.text(8, count + 0, String((float)DHT11.temperature, 1));
    myScreen.text(12, count + 0, String("C"));

    myScreen.text(0, count + 1, "R. Hum :");
    myScreen.text(8, count + 1, String((float)DHT11.humidity, 1));
    myScreen.text(12, count + 1, String("%"));

    count++;
    count = count % 5;

    sleep(60000);
}
