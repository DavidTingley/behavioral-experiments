#include <elapsedMillis.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#define NOFIELD 66L    // Analog output with nU applied field, calibrate this
#define TOMILLIGAUSS 1822L

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(1800, 2); // was 200

int WATER = 0;
unsigned long lastMillis = 0.0;
int tStart = 0;
byte c = ' ';

// WHEEL STUFF
const int wheelSensor = A0;
const int leverSensor = A1;
double wheelValue;
double leverValue;

double velocityThreshold = .012; // was .015 for rig 1
int trialType = 1;
double lastWheel = 0.;
double lastLever = 0.;
double wheelVelocity = 0;
double leverAvg = 0;
int runningTimes[] = {0, 1000};
bool rot = true;
double maxForce = 0;
int leverCount = 0;
int signalPin = 7; // use this pin to send signals to Intan digin

void setup()
{
  Serial.begin(9600);
  pinMode(WATER, OUTPUT);
  digitalWrite(WATER, HIGH);
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(20);  // 10 rpm

  Serial.println("set up and running..");
}

void loop()
{
  // get sensor readings
  int leverValue = analogRead(leverSensor);
  double leverAvg = (leverValue + lastLever * 3) / 4.;

  wheelValue = analogRead(wheelSensor);
  //Serial.println(wheelValue);
  long compensated = wheelValue - NOFIELD;                 // adjust relative to no applied field
  long gauss = compensated * TOMILLIGAUSS / 1000;
  wheelValue = cos((gauss / 1024.0) * 2.0 * 3.1459);     // map it to the range of the analog out:

  if (abs(wheelValue - lastWheel) > 1) // cuts out jumps in phase..
  {
    lastWheel = wheelValue;
  }
  else
  {
    wheelValue = (wheelValue + lastWheel * 2) / 3.;
  }
  wheelVelocity = (abs(wheelValue - lastWheel) + wheelVelocity * 19) / 20;

  // lever is pressed
  if ((leverValue - leverAvg) > 5.)
  {
    double force = (leverValue - leverAvg);
    if (force > maxForce)
    {
      maxForce = force;
    }
    leverCount = leverCount + 1;
    if (leverCount > 2)
    {
      Serial.print("force; ");
      Serial.print(force);
      Serial.println("");
    }
  }
  else
  {
    if (maxForce > 10. && leverCount > 6)
    {
      if (rot == false) {
        myMotor->step(35, FORWARD, MICROSTEP);
        rot = !rot;
        delay(100);
        myMotor->release();
      }
      else if (rot == true) {
        myMotor->step(35, BACKWARD, MICROSTEP);
        rot = !rot;
        delay(100);
        myMotor->release();
      }
      Serial.print("trial: lever pressed, max: ");
      Serial.print(maxForce);
      Serial.print(", duration: ");
      Serial.println(leverCount);
      maxForce = 0;
    }
    leverCount = 0;
  }

  // wheel is moving, begin move into trial
  if (wheelVelocity > velocityThreshold)
  {
    Serial.print("position = ");
    Serial.print(wheelValue, 4);
    Serial.print("\t velocity = ");
    Serial.print(wheelVelocity, 5);
    Serial.print("\t time elapsed = ");
    Serial.println(millis() - lastMillis);

    if (tStart == 0 && millis() - lastMillis > 1000)
    {
      tStart = 1;
    }
  }
  else if (wheelVelocity < velocityThreshold)
  {
    int distFromTarget = runningTimes[trialType] - int(millis() - lastMillis) + 1000;
    if (tStart == 1)
    {
      if (millis() - lastMillis > 1000. && (distFromTarget) <  0.5 * runningTimes[trialType])
      {
        float bonus = (1. - (float(abs(distFromTarget)) / float(runningTimes[trialType]))) * 10.;
        digitalWrite(WATER, LOW);
        delay(int(500 + bonus + runningTimes[trialType] / 100));
        digitalWrite(WATER, HIGH);
        Serial.println(int(500 + bonus));
        Serial.print("trial:");
        Serial.print(trialType);
        Serial.print(" correct, off by: ");
        Serial.print(distFromTarget);
        Serial.print(", target: ");
        Serial.print(runningTimes[trialType]);

        Serial.println("");
      }
      else
      {
        Serial.print(trialType);
        Serial.print(" incorrect, off by: ");
        Serial.print(distFromTarget);
        Serial.print(", ");
        Serial.print("target: ");
        Serial.print(runningTimes[trialType]);
        Serial.println("");
      }
    }
    lastMillis = millis();
    tStart = 0;
    int leverValue = analogRead(leverSensor);
    leverAvg = leverValue;
  }
  // send it to the computer as ASCII digits
  lastWheel = wheelValue;
  lastLever = leverAvg;
  delay(5);
}
