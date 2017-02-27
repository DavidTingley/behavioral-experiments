
#include <elapsedMillis.h>

elapsedMillis timeElapsed;
int sensorPin_Base = A0;    // select the input pin for the potentiometer
int sensorPin_Extension = A1;    // select the input pin for the potentiometer
float irSensor_Base = 0;  // variable to store the value coming from the sensor
float irSensor_Extension = 0;  // variable to store the value coming from the sensor
int solenoid_Base = 6;
int solenoid_Extension = 7;
int startLoc = 0;
int numTrials = 0;
char STARTCODE = ' ';

void setup() {
  Serial.begin(19200);
  pinMode(solenoid_Base, OUTPUT);
  pinMode(solenoid_Extension,OUTPUT);
  digitalWrite(solenoid_Base, HIGH);
  digitalWrite(solenoid_Extension,HIGH);
}

void loop() {
  if (Serial.available())
  {
    STARTCODE = Serial.read();
    if (STARTCODE == 'g')
    {
      Serial.println("we're up and running");
    }
    else
    {
      Serial.println("we are no longer running...");
    }
  }
  if (STARTCODE == 'g')
  {
    // read the value from the sensor:
      irSensor_Base = analogRead(sensorPin_Base);
      irSensor_Extension = analogRead(sensorPin_Extension);
    for (int i = 0;i<5; i++)
    {
      delay(1);
      irSensor_Base = irSensor_Base + analogRead(sensorPin_Base);
      irSensor_Extension = irSensor_Extension + analogRead(sensorPin_Extension);
    }

     
    // turn the ledPin on
  
    if  (irSensor_Base < 600.)
    {
      if (startLoc == 1 && timeElapsed < 8000)
      {
        digitalWrite(solenoid_Base,LOW);
        delay(1000);
        digitalWrite(solenoid_Base,HIGH);
        numTrials++;
        Serial.print(numTrials);
        Serial.print(" trials complete, ");
        Serial.print(timeElapsed);
        Serial.println(" ");
      }
      timeElapsed = 0;
      startLoc = 0;
    }
    if  (irSensor_Extension < 600. && timeElapsed < 80000)
    {
      if (startLoc == 0)
      {
        digitalWrite(solenoid_Extension,LOW);
        delay(1000);
        digitalWrite(solenoid_Extension,HIGH);
        numTrials++;
        Serial.print(numTrials);
        Serial.print(" trials complete, ");
        Serial.print(timeElapsed);
        Serial.println(" ");
      }
      timeElapsed = 0; 
      startLoc = 1;
    }
    delay(1);
//    Serial.print(irSensor_Base);
//    Serial.println(irSensor_Extension);
  }
}
