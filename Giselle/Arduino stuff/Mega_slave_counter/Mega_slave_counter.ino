// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int num = 0;
  
void setup() {
  Serial.begin(9600);
  Serial.println("Mega has started program FOR REAL");
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

}

void loop() {
//  Serial.println("Mega is in loop");
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
//  Serial.println("I am writing");
  Serial.println(num);
//  Wire.write("I am sending ");
  Wire.write(num); // respond with message of 8 bytes
  // as expected by master
  num++;
}
