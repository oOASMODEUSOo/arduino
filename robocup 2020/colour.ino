void senseColour(bool _blink) {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  int R = pulseIn(sensorOut, LOW);
  delay(5);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  int G = pulseIn(sensorOut, LOW);
  delay(5);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  int B = pulseIn(sensorOut, LOW);
  delay(5);

  //  String msg = "";
  //  msg += R;
  //  msg += ",";
  //  msg += G;
  //  msg += ",";
  //  msg += B;
  //  Serial.println(msg);
  if (R <= 20 and G == 2500) {
    //Serial.print("RED....");
    if (_blink) {
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzPin, HIGH);
        delay(100);
        digitalWrite(buzzPin, LOW);
        delay(100);
      }
    }
  } else if (R == 2000 and G <= 35) {
    //Serial.print("GREEN....");
    if (_blink) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzPin, HIGH);
        delay(100);
        digitalWrite(buzzPin, LOW);
        delay(100);
      }
    }
  } else if (R >= 26 and G >= 32 and B >= 25) {
    //Serial.print("BLACK.....");
//    moveMotor(0, 0);
//    delay(50);
    moveMotor(-80, -80);
    delay(350);
    moveMotor(0, 0);
    delay(200);
    //maxCountCheck();
    uturn();
    delay(200);
  } else {
    //Serial.print("WHITE.....");
  }
  //Serial.println(msg);
}
