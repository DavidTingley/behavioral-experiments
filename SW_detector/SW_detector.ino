/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
#include <elapsedMillis.h>
#include "RunningMedian.h"
RunningMedian samples0 = RunningMedian(5);
RunningMedian samples1 = RunningMedian(5);
long count = 0;
float lastMillis = 0;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT); 
  pinMode(11,OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  double sensorValue0 = analogRead(A0);
  double sensorValue1 = analogRead(A1);
  samples0.add(sensorValue0);
  float m0 = samples0.getMedian();
  samples1.add(sensorValue1);
  float m1 = samples1.getMedian();
  float diff = (m0-m1)/5;
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
//  double voltage0 = (voltage0 + m0 * (5.0 / 1023.0))/10;
//  double voltage1 = (voltage1  + m1 * (5.0 / 1023.0))/10;
//   print out the value you read:
  if (diff < -13 & millis()-250 > lastMillis)
  {
      digitalWrite(13,HIGH);
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      delay(10);
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      lastMillis = millis();
//      delay(250);
  }
//  Serial.print(30);
//  Serial.print(" ");
//  Serial.print(m0/10,5);
//  Serial.print(" ");
//  Serial.print(m1/10,5);  
//  Serial.print(" ");
//  Serial.print(diff,5);  
//  Serial.print(" ");  
//  Serial.print(-7);   
//  Serial.print(" ");
//  Serial.println(-12);
}
