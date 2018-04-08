/**

   Arduino Slide

   Created by Alexandre Lavoie, 2018.

   Uses/Inspired By:

   Ultrasonic Sensor HC-SR04 and Arduino Tutorial

   Created by Dejan Nedelkovski,
   www.HowToMechatronics.com

   Arduino Audible Distance Sensor (Reverse Sensor)
   http://blog.afkham.org/2013/02/how-to-build-arduino-audible-distance.html

   Arduino Synth
   https://github.com/dzlonline/the_synth

*/

#include "tables.h" // Includes C file used for memory manipulation to get Sin Tables.

#define NOP __asm__ __volatile__ ("nop\n\t") // Assembly NOP used for delay.

// Defines the pins.
const int triggerPin = 12; // triggerPin used to send a signal to the ultrasonic sensor.
const int listenPin = 11; // listenPin used to receive the signal.
const int switchPin = 9; // switchPin used to receive switch state. (0V = down and 5V = up).

// Defines variables.

const int fretLength = 4; // Length for a fret in centimeters.
sintable sinTable; // Sets variable to use the sintable class.
long duration; // Used to store time taken for pulse to go to the object and come back.
int distance; // Distance to the object in centimeters.

/**
   Function that runs once when the arduino starts.
*/

void setup() {

  DDRC = B11111111; // Sets Port C in output mode.

  pinMode(triggerPin, OUTPUT); // Sets the triggerPin such that it can output information.
  pinMode(listenPin, INPUT); // Sets the listenPin such that it can get information.
  pinMode(switchPin, INPUT); // Sets the switchPin such that it can get information.
  Serial.begin(9600); // Stars the serial communication with frequency.

}

/**
   Function that runs continuously after setup() is ran.
*/

void loop() {

  //If switch is down, then play sound.
  if (digitalRead(switchPin) == 0) {
    PlaySound(); //Plays sound according to distance.
  }

}

/**
   Function that finds the time to the nearest object and back. Object has to be between [2,500] cm for accurate readings.

   Returns the time object and back in microseconds.
*/

long GetTimeObjectAndBack() {

  //Assures that the ardunio is not receiving a pulse before sending a pulse.
  while (digitalRead(listenPin) == 1) {}

  //Clears the trigger pin.
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);

  //Sends a pulse for 10 microseconds to the ultrasonic sensor. This is required by the sensor to send it's 8 pulses.
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(triggerPin, LOW);

  //Waits till it receives a HIGH on the listenPin and gives the time it takes.
  return pulseIn(listenPin, HIGH);

}

/**
   Function gets the distance to the object.

   Knowing that the speed of sound is 340 m/s, conversion factors can be applied to get the speed of sound in cm/microseconds.
   The time taken to go to and from the object is then divided by two to get the time to go to the object. Multiplying this with the speed of sound in cm/microseconds will give us the distance to the object in cm.

   timeTaken is the time that it took to the object and back in microseconds.

   Returns the distance to the object in centimeters.
*/

int GetDistanceToObject(long timeTaken) {

  return (340.0 * (1.0 / 1000000.0) * (100.0) * (duration / 2.0)); // 340 m/s * (1 s/1000000 microseconds) * (100 cm/1 m) * (duration microseconds/2) --> Decimals were added because calculation were made in integers, which yielded 0.

}

/**
   Function that plays a frequency depending on the distance of the object.
*/

void PlaySound() {

  duration = GetTimeObjectAndBack(); //Gets the time it takes to get to the object and back in microseconds and sets it to the variable.

  distance = GetDistanceToObject(duration); //Gets the distance to the object in centimeters.

  int Note = -1; // The note id for the notes array.

  //Tests distance length to see which note is associated to the distance when it falls within a fret length. If it too small or too large of a distance, it will be ignored.
  for (int i = 0; i < 12; i++) {

    // If the distance is between x and x+fretLength.
    if (fretLength * i < distance && fretLength * (i + 1) > distance) {
      Note = i;
      break;
    }
  }

  //If there is a note to be played, it plays the note.

  if (Note != -1) {

    int sizeOf = sinTable.getSize(Note); // Gets the size of the array containing the frequency.
    int samples[sizeOf]; // Sets array in SRAM.
    sinTable.getTable(samples, sizeOf, Note); // Sets the samples array with the points of the sin curve inside the PROGMEM.

    // While the button is down, repeat.
    while (digitalRead(switchPin) == 0) {

      int CurrentSample = 0; // Counter. Like a for loop, but while loop is deemed faster which is why it is used.

      // While the Current Sample is less than the size of the array containing the points of the sin curve.
      while (CurrentSample < sizeOf) {
        PORTC = samples[CurrentSample]; // Sets PORTC to the correct byte frequency to be sent to the R-2R ladder.
        delayMicroseconds(15); // Waits 15 microseconds.
        NOP; NOP; NOP; NOP; NOP; NOP; NOP; // Adds extra delay for more precision.
        CurrentSample++; // Increments the counter by one.
      }
    }
  }
}
