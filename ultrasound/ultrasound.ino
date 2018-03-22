/**
 * Created by Alexandre Lavoie, 2018.
 * 
 * Uses:
 * 
 * Ultrasonic Sensor HC-SR04 and Arduino Tutorial
 * 
 * Created by Dejan Nedelkovski, 
 * www.HowToMechatronics.com
 * 
 * Arduino Audible Distance Sensor (Reverse Sensor)
 * http://blog.afkham.org/2013/02/how-to-build-arduino-audible-distance.html
 * 
 */

// Defines the note frequencies.

#define NOTE_C4 262
#define NOTE_C4S 277
#define NOTE_D4 293
#define NOTE_D4S 311
#define NOTE_E4 329
#define NOTE_F4 349
#define NOTE_F4S 367
#define NOTE_G4 392
#define NOTE_G4S 415
#define NOTE_A4 440
#define NOTE_A4S 466
#define NOTE_B4 494

// Defines the pins.
const int triggerPin = 12; // triggerPin used to send a signal to the ultrasonic sensor.
const int listenPin = 11; // listenPin used to receive the signal.
const int soundPin = 10; // soundPin used to send pulse to speaker.

// Defines variables.
long duration; // Used to store time taken for pulse to go to the object and come back.
int distance; // Distance to the object in centimeters.
const int notes[12] = {NOTE_C4,NOTE_C4S,NOTE_D4,NOTE_D4S,NOTE_E4,NOTE_F4,NOTE_F4S,NOTE_G4,NOTE_G4S,NOTE_A4,NOTE_A4S,NOTE_B4}; // All notes in the C major scale.
const int fretLength = 4; // Length for a fret in centimeters.

/**
 * Function that runs once when the arduino starts.
 */

void setup() {
  pinMode(triggerPin, OUTPUT); // Sets the triggerPin such that it can output information.
  pinMode(listenPin, INPUT); // Sets the listenPin such that it can get information.
  pinMode(soundPin, OUTPUT); // Set the soundPin such that it can output its frequencies.
  Serial.begin(9600); // Stars the serial communication with frequency.
}

/**
 * Function that runs continuously after setup() is ran.
 */

void loop() {
  
  duration = GetTimeObjectAndBack(); //Gets the time it takes to get to the object and back in microseconds and sets it to the variable.

  distance = GetDistanceToObject(duration); //Gets the distance to the object in centimeters.

  PlaySound(distance,2); //Plays sound according to distance.
  
}

/**
 * Function that finds the time to the nearest object and back. Object has to be between [2,500] cm for accurate readings.
 * 
 * Returns the time object and back in microseconds.
 */

long GetTimeObjectAndBack(){
  
  //Clears the trigger pin.
  digitalWrite(triggerPin,LOW);
  delayMicroseconds(2);

  //Sends a pulse for 10 microseconds to the ultrasonic sensor. This is required by the sensor to send it's 8 pulses.
  digitalWrite(triggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);

  //Waits till it receives a HIGH on the listenPin and gives the time it takes.
  return pulseIn(listenPin, HIGH);
  
}

/**
 * Function gets the distance to the object. 
 * 
 * Knowing that the speed of sound is 340 m/s, conversion factors can be applied to get the speed of sound in cm/microseconds. 
 * The time taken to go to and from the object is then divided by two to get the time to go to the object. Multiplying this with the speed of sound in cm/microseconds will give us the distance to the object in cm.
 * 
 * timeTaken is the time that it took to the object and back in microseconds.
 * 
 * Returns the distance to the object in centimeters.
 */

int GetDistanceToObject(long timeTaken){
  
  return (340.0 * (1.0/1000000.0) * (100.0) * (duration / 2.0)); // 340 m/s * (1 s/1000000 microseconds) * (100 cm/1 m) * (duration microseconds/2) --> Decimals were added because calculation were made in integers, which yielded 0.
  
}

/**
 * Function that plays a frequency depending on the distance of the object.
 * 
 * distanceTo is the distance to the object in centimeters.
 * noteDuration in seconds.
 * 
 */

void PlaySound(int distanceTo, int noteDuration){

  int Note = 0; // The note id for the notes array.

  //Tests distance length to see which note is associated to the distance when it falls within a fret length. If it too small or too large of a distance, it will be ignored.

  for(int i=0; i < 12; i++){
    if(fretLength*i<distanceTo&&fretLength*(i+1)>distanceTo){
        Note = i;
        break;
      }
  }

  //If there is a note to be played, it plays the note.
  
  if(Note!=0){
    tone(soundPin, notes[Note], 1000/noteDuration);
    
    delay(1000/noteDuration * 1.3);

    noTone(soundPin);
  }
  
}