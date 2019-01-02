## CarSensor

  Arduino IDE Library to read cheap ultrasonic car parking sensors.

  NOTE: This repository still under construction. Soon it will have complete 
  information about the module hardware and data formats.

```c
#include "CarSensor.h"

#define PIN_MODULE 4

CarSensor carModule(PIN_MODULE);

void setup() {
    Serial.begin(115200);
    Serial.println("Module Begin");
    carModule.begin(); // initialize and calibrate
}

void loop() {
    carModule.read(); // get data from module
    int sensor1 = carModule.getSensor(0);
}

```

## The module

The module is a China version of a rear parking ultrasonic sensor. It costs about 10$ for the full kit. It comes with 4 ultrasonic sensors, a module to connect the sensors and a display with a annoying buzzer. It it supposed to be installed on cars without the sensor kit. One should drill a hole into the bumber and connect the module to the Reverse lights 12V.

## Pulse Train

The pulse train consists of 50 bits. The first 4 bytes are the sensors readings. 
They come in a strange order:

Pulse train example:
00000000 00000000 00000000 00000000 000010010 11 00 00 00

* First byte  - sensor A
* Second byte - sensor C
* Third byte  - sensor B
* Fourth byte - sensor D

## Compatibility

	* Arduinos (UNO & Pro-micro tested)
	* ESP32 (Lolin32 and ESP32 Dev. Module tested). 
	Remember, these modules are 3.3V. Be sure to level shift 5v to 3.3v 
	or make a voltage divider with 2 resistors.




