// By Clarence Ho (2015)
// TM4C123 connected to Nokia 5110 LCD display and DHT11 sensor
//
// Nokia LCD 5110 part based on sample of LCD_5110_SPI library
//
// Wiring (using the third SPI port on TM4C123):
// TM4C123 -  LCD 5110        Comment
// ==================================
// VBUS    -  Vcc             My version of 5110 supports 3v to 5v
// VBUS    -  BL              Backlight
// GND     -  GND
// PB_5    -  RST             Reset 
// PB_4    -  Clk             SCK(2) to Clock
// PB_7    -  Din             MOSI(2) to Serial data in
// PA_7    -  CE              Chip Select
// PA_2    -  DC              Select between data or command


// TM4C123 -  DHT11        Comment
// ==================================
// PD_7    -  Data
// VBUS    -  Vcc
// GND     -  GND

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

LCD_5110_SPI myScreen(PA_7,    // Chip Select
                      PA_2,    // Data/Command
                      PB_5,    // Reset
                      PA_6,    // Backlight - not used in my program
                      PUSH2);  // Push Button 2 - not used in my program

dht11 DHT11;
// DHT11 data pin
#define DHT11_PIN PD_7

volatile uint8_t count = 0;

// Add setup code
void setup() {
    SPI.setModule(2);      // using the third SPI port
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV128); // for LM4F120H5QR DIV2 = 4 MHz !
    
    myScreen.begin();
    myScreen.setContrast(0x28);
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
        myScreen.text(0, 0, "Problem reading sensor. Wait...");
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
