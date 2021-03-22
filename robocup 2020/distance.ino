
int getDistance(int pingPin) {

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  int duration = pulseIn(pingPin, HIGH, 50000);
  int distance = duration * 0.034 / 2;
  delay(10);
//  if (distance <= 0) {
//    getDistance(pingPin);
//  } else
    return distance;
}

void maxCountCheck() {
  COUNT++;
  if (COUNT == MAXCOUNT) {
    while (1) {
      digitalWrite(buzzPin, HIGH);
      delay(100);
      digitalWrite(buzzPin, LOW);
      delay(100);
    }
  }
}
