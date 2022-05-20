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
    delay(3);
  }
}

void closeClaw() {
  //Serial.println("Close claw\n");

  for (armPositionClaw; armPositionClaw > 0; armPositionClaw--) {
    armServoClaw.write(armPositionClaw);
    delay(3);
  }
}

// armPositionLeft: 0 - вытянута, 140 - втянута
void armForward() {
  int targetPos = armPositionLeft - 10;
  
  if (targetPos < armPositionLeftMin) {
    targetPos = armPositionLeftMin;
  }

  for (armPositionLeft; armPositionLeft > targetPos; armPositionLeft--) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  Serial.println("Left: " + String(armPositionLeft) + "; Right: " + String(armPositionRight));
}

void armBack() {
  int targetPos = armPositionLeft + 10;
  
  if (targetPos > armPositionLeftMax) {
    targetPos = armPositionLeftMax;
  }

  for (armPositionLeft; armPositionLeft < targetPos; armPositionLeft++) {
    armServoLeft.write(armPositionLeft);
    delay(5);
  }

  Serial.println("Left: " + String(armPositionLeft) + "; Right: " + String(armPositionRight));
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  int targetPos = armPositionRight - 10;
  
  if (targetPos < armPositionRightMin) {
    targetPos = armPositionRightMin;
  }

  for (armPositionRight; armPositionRight > targetPos; armPositionRight--) {
    armServoRight.write(armPositionRight);
    delay(5);
  }

  Serial.println("Left: " + String(armPositionLeft) + "; Right: " + String(armPositionRight));
}

void armDown() {
  int targetPos = armPositionRight + 10;
  
  if (targetPos > armPositionRightMax) {
    targetPos = armPositionRightMax;
  }

  for (armPositionRight; armPositionRight < targetPos; armPositionRight++) {
    armServoRight.write(armPositionRight);
    delay(5);
  }

  Serial.println("Left: " + String(armPositionLeft) + "; Right: " + String(armPositionRight));
}


// <<< Arm functions
