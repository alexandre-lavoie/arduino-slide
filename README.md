# Arduino Slide

Uses the arduino ultrasonic sensor to produce frequencies from a library on a speaker.

# Getting Started

* An Aruino Board (MEGA 2560).
* An Ultrasonic Sensor (Model HC-SR04).
* A Speaker, Headphone Jack, or Piezobuzzer.
* A R-2R Ladder DAC

> Note that the ultrasonic sensor that was used has a trigger (pin 12 on arduino) and echo (pin 11 on arduino) pin. 

![alt text](https://raw.githubusercontent.com/alexandre-lavoie/arduino-slide/final/images/circuit.png)

# Current Implementations

* Find the distance to an object.
* Play a note according to the distance to an object.
* Data table with sin curve points for various frequencies.

# Reference Works

1. www.HowToMechatronics.com - Dejan Nedelkovski
2. http://blog.afkham.org/2013/02/how-to-build-arduino-audible-distance.html - Afkham Azeez
3. https://github.com/dzlonline/the_synth - dzlonline