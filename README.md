# TM4C123 connects to Nokia 5110 LCD display and DHT11 sensor

![photo](http://3.bp.blogspot.com/-kUpUaPbtTd0/VcfgK1y8taI/AAAAAAAACT8/5ABSUtC2I9o/s320/IMAG1230.jpg "photo")


Display temperature and humidity on Nokia 5110 screen, driven by TM4C123.  Programs written with Energia utilizing appropriate libraries.

References:
- DHT11: http://www.adafruit.com/product/386
- DHT11 library: http://playground.arduino.cc/main/DHT11Lib
- Nokia 5110 LCD library: http://forum.43oh.com/topic/2876-energia-library-nokia-5110-lcd-boosterpackbreakout-pcb/

Note that the Nokia 5110 library is not included in the Energia release.  But you can get source code from github:
https://github.com/energia/Energia/tree/master/examples/07.Display

Both libraries are slightly modified to work with TM4C123.

Here is the wiring.  Note that my Nokia 5110 board support input of 3v to 5v.  Your mileage may vary.
```
TM4C123 -  LCD 5110        Comment
==================================
VBUS    -  Vcc             My version of 5110 supports 3v to 5v
VBUS    -  BL              Backlight
GND     -  GND
PB_5    -  RST             Reset 
PB_4    -  Clk             SCK(2) to Clock
PB_7    -  Din             MOSI(2) to Serial data in
PA_7    -  CE              Chip Select
PA_2    -  DC              Select between data or command



TM4C123 -  DHT11        Comment
==================================
PD_7    -  Data
VBUS    -  Vcc
GND     -  GND
```
