// Arm functions >>>

void armTurnRight() {
  //Serial.println("Turn right\n");

  int targetPos = armPositionMain - 10;

  if (targetPos < armPositionMainMin) {
    targetPos = armPositionMainMin;
  }

  for (armPositionMain; armPositionMain > targetPos; armPositionMain--) {
    armServoMain.write(armPositionMain);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

void armTurnLeft() {
  //Serial.println("Turn left\n");

  int targetPos = armPositionMain + 10;

  if (targetPos > armPositionMainMax) {
    targetPos = armPositionMainMax;
  }

  for (armPositionMain; armPositionMain < targetPos; armPositionMain++) {
    armServoMain.write(armPositionMain);
    delay(5);
  }

  delay(100);
}

void armTurnCenter() {
  while (armPositionMain < armPositionMainCenter) {
    armTurnLeft();
  }

  while (armPositionMain > armPositionMainCenter) {
    armTurnRight();
  }
}

void openClaw() {
  //Serial.println("Open claw\n");

  for (armPositionClaw; armPositionClaw < 50; armPositionClaw++) {
    armServoClaw.write(armPositionClaw);
  }

  delay(100);
}

void closeClaw() {
  //Serial.println("Close claw\n");

  for (armPositionClaw; armPositionClaw > 0; armPositionClaw--) {
    armServoClaw.write(armPositionClaw);
  }

  delay(100);
}

void liftUp() {
  //Serial.println("Lift up\n");

  for (armPositionLeft; armPositionLeft < 120; armPositionLeft++) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  delay(1000);
}

void liftDown() {
  //Serial.println("Lift down\n");

  for (armPositionLeft; armPositionLeft > 50; armPositionLeft--) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  delay(1000);
}

void leftServo5() {
  //Serial.println("left servo rotates to 5 degrees\n");

  for (armPositionLeft; armPositionLeft > 50; armPositionLeft--) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  delay(1500);
}

void rightServo100() {
  // right servo rotates to 100 degrees
  for (armPositionRight; armPositionRight > 50; armPositionRight--) {
    armServoRight.write(armPositionRight);
    delay(5);
  }
}

void leftServo120() {
  //Serial.println("left servo rotates to 100 degrees, rocker arm lifts\n");
  for (armPositionLeft; armPositionLeft < 120; armPositionLeft++) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  delay(1000);
}

// armPositionLeft: 0 - вытянута, 140 - втянута
void armForward() {
  //Serial.println("Arm forward\n");

  int minPos = 0;
  int targetPos = armPositionLeft - 46;

  if (targetPos < minPos) {
    targetPos = minPos;
  }

  for (armPositionLeft; armPositionLeft > targetPos; armPositionLeft--) {
    armServoLeft.write(armPositionLeft);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

void armBack() {
  //Serial.println("Arm back\n");

  int maxPos = 140;
  int targetPos = armPositionLeft + 46;

  if (targetPos > maxPos) {
    targetPos = maxPos;
  }

  for (armPositionLeft; armPositionLeft < targetPos; armPositionLeft++) {
    armServoLeft.write(armPositionLeft);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  //Serial.println("Arm up\n");

  int minPos = 20;
  int targetPos = armPositionRight - 40;

  if (targetPos < minPos) {
    targetPos = minPos;
  }

  //Serial.println("Arm up to " + String(targetPos));

  for (armPositionRight; armPositionRight > targetPos; armPositionRight--) {
    //Serial.println("Arm right " + String(armPositionRight));
    armServoRight.write(armPositionRight);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

void armDown() {
  //Serial.println("Arm down\n");

  int maxPos = 80;
  int targetPos = armPositionRight + 40;

  if (targetPos > maxPos) {
    targetPos = maxPos;
  }

  //Serial.println("Arm down to " + String(targetPos));

  for (armPositionRight; armPositionRight < targetPos; armPositionRight++) {
    Serial.println("Arm right " + String(armPositionRight));
    armServoRight.write(armPositionRight);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

// <<< Arm functions
