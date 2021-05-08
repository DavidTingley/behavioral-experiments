/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
#include <elapsedMillis.h>
#include "RunningMedian.h"
#include <TimeLib.h>
RunningMedian samples0 = RunningMedian(5);
RunningMedian samples1 = RunningMedian(5);
long count = 0;
int stimOn = 1;
float Time = 0;
float lastMillis = 0;
int threshold = -14;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  //  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  //  pinMode(9, OUTPUT);
  //  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.println("starting with stim ON");
}

// the loop routine runs over and over again forever:
void loop() {

  // read the input on analog pin 0:
  double sensorValue0 = analogRead(A0);
  double sensorValue1 = analogRead(A8);
  samples0.add(sensorValue0);
  float m0 = samples0.getMedian();
  samples1.add(sensorValue1);
  float m1 = samples1.getMedian();
  float diff = (m0 - m1) / 5;
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //  double voltage0 = (voltage0 + m0 * (5.0 / 1023.0))/10;
  //  double voltage1 = (voltage1  + m1 * (5.0 / 1023.0))/10;
  //   print out the value you read:
  if (diff < threshold & millis() - 250 > lastMillis & stimOn == 1)
  {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    delay(10);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    lastMillis = millis();
    //      delay(250);
  }
  else if (diff < threshold & millis() - 250 > lastMillis & stimOn == 0)
  {
    digitalWrite(6, HIGH);
    delay(10);
    digitalWrite(6, LOW);
    lastMillis = millis();
  }


  if (hour() % 2 == 0 & minute() == 0 &second() == 0 & hour() > 0)
  {
    if (stimOn == 1)
    {
      digitalWrite(13, LOW);
      stimOn = 0;
//      Serial.println("switching stim off");
//      Serial.println(hour());
    }
    else
    {
      digitalWrite(13, HIGH);
      stimOn = 1;
//      Serial.println("switching stim on");
//      Serial.println(hour());
    }
    delay(1000 * 2);
  }
  //  Serial.print(0);
    Serial.print(" ");
    Serial.print(sensorValue0 / 10, 5);
    Serial.print(" ");
    Serial.print(sensorValue1 / 10, 5);
    Serial.print(" ");
    Serial.print(diff, 5);
    Serial.print(" ");
    Serial.print(threshold);
    Serial.print(" ");
    Serial.println(" ");
}
