#include <elapsedMillis.h>
#define speakerPin 3
#define START_CODE 'g'
#define NOFIELD 66L    // Analog output with no applied field, calibrate this
#define TOMILLIGAUSS 1822L

int WATER = 2;
unsigned long lastMillis = 0.0;
int tStart = 0;
byte c = ' ';

// WHEEL STUFF
const int wheelSensor = A0;
double sensorValue;
double velocityThreshold = .01;
int trialType = 1;
double lastSensor;
double wheelVelocity = 0;
float nIncorrect[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
float nCorrect[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
int toneFrequencies[] = {3000,6000,12000};
int runningTimes[] = {0,4000};
//float performance[] = {.22,.5,.675};
float performance[] = {.8,.8,.8,.8,.8,.8,.8,.8,.8,.8};
void setup()
{
  Serial.begin(28800);
  pinMode(WATER, OUTPUT);
  digitalWrite(WATER, HIGH);
}

void loop()
{
  if (Serial.available() > 0)
  {
    c = Serial.read();
    lastMillis = millis();
    Serial.println("we're up and running..");
  }
  //  static boolean sampling = false;
  if (c == START_CODE)
  {
    sensorValue = analogRead(wheelSensor);
    long compensated = sensorValue - NOFIELD;                 // adjust relative to no applied field 
    long gauss = compensated * TOMILLIGAUSS / 1000; 
    sensorValue = cos((gauss / 1024.0) * 2.0 * 3.1459);     // map it to the range of the analog out:
    sensorValue = (sensorValue + lastSensor * 2) / 3.;
    wheelVelocity = (abs(sensorValue - lastSensor) + wheelVelocity * 4) / 5;
   
    if (wheelVelocity > velocityThreshold)
       {
      Serial.print("position = ");
      Serial.print(sensorValue, 4);
      Serial.print("\t velocity = ");
      Serial.print(wheelVelocity, 5);
      Serial.print("\t time elapsed = ");
      Serial.println(millis() - lastMillis);

      if (tStart == 0 && millis() - lastMillis > 1000)
      {
//        tone(speakerPin, toneFrequencies[trialType], 500);
//        if (trialType==0) { // FM downsweep 5kHz-2kHz
//          for (int i = 5000; i >= 2000; i=i-10) {
//          tone(speakerPin, i,2);
//          delay(1);
//          }
//        }
//        if (trialType==1){ // white noise 2kHz-11kHz
//          for (int i = 0; i <= trialType; i=i+1) {
//          int r = random(50,150);
//          int wNoise = random(2000,11000);
//          tone(speakerPin, 10000 ,20);
//          delay(r);
//          }
//        }
//        if (trialType==2){ // FM upsweep 8kHz-11kHz
//          for (int i = 8000; i <= 11000; i=i+10) {
//          tone(speakerPin, i,2);
//          delay(1);
//          }
//        }
        tStart = 1;
      }
    }
    else if (wheelVelocity < velocityThreshold)
    {
      int distFromTarget = runningTimes[trialType] - int(millis() - lastMillis) + 1000;
      if (tStart == 1)
      {
        if (millis() - lastMillis > 1000. && (distFromTarget) < int(performance[trialType] * runningTimes[trialType])) 
        {
          float bonus = (1. - (float(abs(distFromTarget)) / float(runningTimes[trialType]))) * 10.;
          digitalWrite(WATER, LOW);
          delay(int(50 + bonus + runningTimes[trialType]/100));
          digitalWrite(WATER, HIGH);
          nCorrect[trialType] = nCorrect[trialType] + 1;
          performance[trialType] = performance[trialType] - .04; // increment performance (error rate) by correct
          if (performance[trialType] < .5)
          {
            runningTimes[trialType] = runningTimes[trialType] + 10;
          }
          Serial.println(int(25 + bonus));
          Serial.print(trialType);
          Serial.print(" correct, off by: ");
          Serial.print(distFromTarget);
          Serial.print(" target: ");
          Serial.print(runningTimes[trialType]);
          Serial.print(", ");
          Serial.print(nCorrect[trialType]);
          Serial.print(" correct, ");
          Serial.print(nIncorrect[trialType]);
          Serial.print(" incorrect, performance = ");
//          for (int i = 0; i < 10; i++)
//          {
            Serial.print(performance[trialType],3);
//            Serial.print(", ");
//          }
          Serial.println("");
          delay(random(runningTimes[trialType] / 2., runningTimes[trialType]));
        }
        else
        {
          nIncorrect[trialType] = nIncorrect[trialType] + 1;
//          performance[trialType] = (nIncorrect[trialType] / (nCorrect[trialType] + nIncorrect[trialType]));
          performance[trialType] = performance[trialType] + .01; // increment performance (error rate) by incorrect
          if (performance[trialType] > 1.){
            performance[trialType] = 1.;
          }
//          if (performance[trialType] > .5)
//          {
//            runningTimes[trialType] = runningTimes[trialType] - 1;
//          }
          Serial.print(trialType);
          Serial.print(" incorrect, off by: ");
          Serial.print(distFromTarget);
          Serial.print(", ");
          Serial.print("target: ");
          Serial.print(runningTimes[trialType]);
          Serial.print(", ");
          Serial.print(nCorrect[trialType]);
          Serial.print(" correct, ");
          Serial.print(nIncorrect[trialType]);
          Serial.print(" incorrect, performance = ");
//          for (int i = 0; i < 10; i++)
//          {
            Serial.print(performance[trialType],3);
//            Serial.print(", ");
//          }
          Serial.println("");
          delay(random(runningTimes[trialType] / 2., runningTimes[trialType]));
        }
//         if (trialType == 8)
//      {
//      trialType = 1;
//      }
//      else
//      {
//      trialType = trialType + 1;
//      }
      }
      lastMillis = millis();
      tStart = 0;
     
    }
    // send it to the computer as ASCII digits
    lastSensor = sensorValue;
    delay(5);
  }
}


