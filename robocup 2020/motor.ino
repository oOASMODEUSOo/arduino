void moveMotor(int LM, int RM) {
  RM *= 1.00; //correction for offset in right motor rpm
  if (LM > 0) {
    analogWrite(LMF, LM);
    analogWrite(LMR, 0);
  }
  else {
    analogWrite(LMF, 0);
    analogWrite(LMR, -LM);
  }

  if (RM > 0) {
    analogWrite(RMF, RM);
    analogWrite(RMR, 0);
  }
  else {
    analogWrite(RMF, 0);
    analogWrite(RMR, -RM);
  }
}

void turnLeft() {
  moveMotor(-160, 160);
  turnGyro(90);
  moveMotor(160, -160);
  delay(30);
  moveMotor(0, 0);
}

void turnRight() {
  moveMotor(160, -160);
  turnGyro(90);
  moveMotor(-160, 160);
  delay(30);
  moveMotor(0, 0);
}

void uturn() {
  moveMotor(-160, 160);
  turnGyro(184);
  moveMotor(160, -160);
  delay(30);
  moveMotor(0, 0);
}

void backAlign() {
//  delay(500);
//  moveMotor(-80, -80);
//  delay(800);
//  moveMotor(80, 80);
//  delay(150);
//  moveMotor(0, 0);
//  delay(100);
}

bool detectAngle() {
  IMUloop();
  if (abs(accY) > RAMPAMGLE)
    return true;
  else
    return false;
}

void turnGyro(int deg) {
  yaw = 0;
  roll = 0;
  pitch = 0;
  while (1) {
    IMUloop();
    if (abs(yaw) > deg) break;
    delay(9);
  }
}

void correctTilePlacement() {
  if (FRONTDISTANCE < 80) {
    int count = 0;
    while (1) {
      int err = FRONTDISTANCE % TILESIZE;
      int factor = 100;
      if (err > 15) {
        int tmp = err - 36;
        moveMotor(constrain(tmp * factor, -160, 160), constrain(tmp * factor, -160, 160));
      } else {
        err -= 6;
        moveMotor(constrain(err * factor, -160, 160), constrain(err * factor, -160, 160));
      }
      if (err >= 0 and err <= 1) {
        //break;
        count++;
        delay(20);
      }
      if (count > 7) {
        break;
      }
    }
  }
}



void moveStraightAligned() {

  int ld = getDistance(LEFT);
  int rd = getDistance(RIGHT);

  int error;

  //both wall present
  if (ld < WALLDISTANCE && rd < WALLDISTANCE) {
    error = (RIGHTDISTANCE - LEFTDISTANCE) * ERRFACTOR;
  }

  //Left wall present
  if (ld < WALLDISTANCE && rd > WALLDISTANCE) {
    error = (10 - LEFTDISTANCE) * ERRFACTOR;
  }

  //Right wall present
  if (ld > WALLDISTANCE && rd < WALLDISTANCE) {
    error = (RIGHTDISTANCE - 10) * ERRFACTOR;
  }

  //No wall present
  if (ld > WALLDISTANCE && rd > WALLDISTANCE) {
    error = 0;
  }

  int SPEED = 140;
  error = constrain(error, -100, 100);
  int lm = SPEED + error;
  int rm = SPEED - error;


  lm = constrain(lm, -SPEED * 1.5, SPEED * 1.5);
  rm = constrain(rm, -SPEED * 1.5, SPEED * 1.5);

  //drive motor here:
  moveMotor(lm, rm);
}


void bumpAlign() {
  if (!digitalRead(LCLICK)) {
    moveMotor(-80, -80);
    delay(200);
    moveMotor(0, 0);
    delay(200);
    moveMotor(-70, -200);
    delay(300);
    moveMotor(200, 70);
    delay(300);
    moveMotor(70, 200);
    delay(500);
  }
  if (!digitalRead(RCLICK)) {
    moveMotor(-80, -80);
    delay(200);
    moveMotor(0, 0);
    delay(200);
    moveMotor(-200, -70);
    delay(300);
    moveMotor(70, 200);
    delay(300);
    moveMotor(200, 70);
    delay(500);
  }
}
