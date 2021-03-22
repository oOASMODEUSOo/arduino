
#include <Wire.h>
#include "config.h"

//Macros
#define LEFTDISTANCE getDistance(LEFT)
#define FRONTDISTANCE getDistance(MIDDLE)
#define RIGHTDISTANCE getDistance(RIGHT)


#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


bool greenDetected    = false;
bool redDetected      = false;

int buzzPin           = 4;

int curDistance       = 0;

int COUNT = 0;

void setup() {
  pinMode(LMF, OUTPUT);
  pinMode(LMR, OUTPUT);
  pinMode(RMF, OUTPUT);
  pinMode(RMR, OUTPUT);

  pinMode(buzzPin, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(LCLICK, INPUT_PULLUP);
  pinMode(RCLICK, INPUT_PULLUP);


  // Setting frequency-scaling to 100%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);


  Serial.begin(115200);

#ifdef DEBUG
  while (1) {
    IMUloop();
    String msg = "";
    //    msg += LEFTDISTANCE;
    //    msg += ",";
    //    msg += FRONTDISTANCE;
    //    msg += ",";
    //    msg += RIGHTDISTANCE;
    //    msg += ",";
    //    msg += digitalRead(LCLICK);
    //    msg += ",";
    //    msg += digitalRead(RCLICK);
    //    msg += ",";
    msg += accY;
    msg += ",";
    msg += detectAngle();
    Serial.println(msg);
    delay(50);
  }
#endif

  //wait for front ultrasonic to stabilize.
  for (int i = 0; i < 10; i++) {
    int x = FRONTDISTANCE;
    delay(50);
  }

  //Wait for starting signal
  while (1) {
    digitalWrite(buzzPin, HIGH);
    delay(50);
    digitalWrite(buzzPin, LOW);
    delay(100);
    Serial.println("waiting...");
    if (FRONTDISTANCE < 5)
      break;
  }

  delay(200);
  beginMotion(); //Initialize IMU MPU-6050 module
  delay(200);
  CalibrateMPU6050_gryo(50); //Calibrate Gyroscope
  delay(200);
  CalibrateMPU6050_Acc(50); //Calibrate accelerometer
  delay(200);

  curDistance = FRONTDISTANCE;
  previousMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  moveStraightAligned();

  bumpAlign();

  //sense colour during normal run ( only for detecting hazard area )
  senseColour(false);

  //if ((FRONTDISTANCE < 4) or (currentMillis - previousMillis >= TILERUN)) {
  if ((currentMillis - previousMillis >= TILERUN)) {

    if (detectAngle()) {
      moveMotor(0, 0);
      delay(100);
      while (1) {
        moveStraightAligned();
        IMUloop();
        if (abs(accY) < RAMPAMGLE) {
          break;
        }
      }
      delay(200);
      moveMotor(-80, -80);
      delay(10);
      moveMotor(0, 0);
    } else {
      moveMotor(-80, -80);
      delay(10);
      moveMotor(0, 0);
      if (FRONTDISTANCE < 100 and FRONTDISTANCE > 5) {
        //correction algorithm
        correctTilePlacement();
      }
    }


    //Stop the motors before checking the cases
    moveMotor(0, 0);
    delay(500);

    //all cases

    bool lw = (LEFTDISTANCE < WALLDISTANCE and LEFTDISTANCE > 0) ? true : false;
    bool fw = (FRONTDISTANCE < WALLDISTANCE and FRONTDISTANCE > 0) ? true : false;
    bool rw = (RIGHTDISTANCE < WALLDISTANCE and RIGHTDISTANCE > 0) ? true : false;

    if (!lw and !fw and !rw) {        //no wall
      //turn left
      turnLeft();
    } else if (!lw and !fw and rw) {   //wall on right
      //turn left and back align
      turnLeft();
      backAlign();
    } else if (!lw and fw and !rw) {   //wall on front
      //turn left
      turnLeft();
    } else if (!lw and fw and rw) {    //wall on right and front
      //turn left and back align
      turnLeft();
      backAlign();
    } else if (lw and !fw and !rw) {   //wall on left
      //move straight.. nothing to be done here
    } else if (lw and !fw and rw) {    //wall on left and right
      //move straight.. nothing to be done here
    } else if (lw and fw and !rw) {    //wall on left and front
      //turn right
      turnRight();
      backAlign();
    } else if (lw and fw and rw) {     //wall on left, front and right
      //turn right twice or u-turn
      //maxCountCheck();
      uturn();
      delay(200);
      backAlign();
    }

    digitalWrite(buzzPin, HIGH);
    delay(100);
    digitalWrite(buzzPin, LOW);
    delay(100);

    senseColour(true);

    //reset current distance
    //curDistance = FRONTDISTANCE;
    previousMillis = millis();
  } //end of tile
} //end of main loop
