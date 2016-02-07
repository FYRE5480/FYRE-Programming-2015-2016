// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int val;
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
//#define LEDPin 13 // Onboard LED

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
  
void setup() {
  Serial.begin(9600);  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
//  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

}

void loop() {
  //  Serial.println("Mega is in loop");
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  
  if (distance >= maximumRange || distance <= minimumRange){
  /* Send a negative number to computer to indicate "out of range" */
  Serial.println("-1");
    val = -1;
  }
  else {
  /* Send the distance to the computer using Serial protocol to indicate successful reading. */
  Serial.println(distance);
    val = distance;
  }
  
  //Delay 50ms before next reading.
  delay(500);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Serial.println("I am writing");
  Serial.println(val);
  Wire.write(val); // respond with message of 8 bytes
  // as expected by master
}
