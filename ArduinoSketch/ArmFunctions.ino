// Arm functions >>>

void initializeArm() {
  ServoPositions servoPositionsFromEeprom = {-1, -1, -1};
  EEPROM.get(0, servoPositionsFromEeprom);

  if (servoPositionsFromEeprom.armMain >= ARM_POSITION_MAIN_MIN && servoPositionsFromEeprom.armMain <= ARM_POSITION_MAIN_MAX) {
    servoPositions.armMain = servoPositionsFromEeprom.armMain;
  }

  armServoMain.attach(PIN_ARM_MAIN);
  armServoMainRotateToPosition("initialize main");
      
  if (servoPositionsFromEeprom.armRight >= ARM_POSITION_RIGHT_MIN && servoPositionsFromEeprom.armRight <= ARM_POSITION_RIGHT_MAX) {
    servoPositions.armRight = servoPositionsFromEeprom.armRight;
  }

  armServoRight.attach(PIN_ARM_RIGHT);
  armServoRightRotateToPosition("initialize right");

  if (servoPositionsFromEeprom.armLeft >= ARM_POSITION_LEFT_MIN && servoPositionsFromEeprom.armLeft <= ARM_POSITION_LEFT_MAX) {
    servoPositions.armLeft = servoPositionsFromEeprom.armLeft;
  }

  armServoLeft.attach(PIN_ARM_LEFT);
  armServoLeftRotateToPosition("initialize left");

  if (servoPositionsFromEeprom.armClaw >= CLAW_POSITION_MIN && servoPositionsFromEeprom.armClaw <= CLAW_POSITION_MAX) {
    servoPositions.armClaw = servoPositionsFromEeprom.armClaw;
  }

  armServoClaw.attach(PIN_ARM_CLAW);
  armServoClawRotateToPosition("initialize claw");
}

void armServoMainRotateToPosition(String caller) {
  if (servoPositions.armMain >= ARM_POSITION_MAIN_MIN && servoPositions.armMain <= ARM_POSITION_MAIN_MAX) {
    Serial.println(caller + ": servo main " + String(servoPositions.armMain));
    armServoMain.write(servoPositions.armMain);
    EEPROM.put(0, servoPositions);
  }
}

void armServoMainRotateToPositionWithoutEeprom() {
  if (servoPositions.armMain >= ARM_POSITION_MAIN_MIN && servoPositions.armMain <= ARM_POSITION_MAIN_MAX) {
    armServoMain.write(servoPositions.armMain);
  }
}

void armServoMainRotateSlowToPosition(int desiredPosition) {
  if (desiredPosition < ARM_POSITION_MAIN_MIN) {
    desiredPosition = ARM_POSITION_MAIN_MIN;
  } else if (desiredPosition > ARM_POSITION_MAIN_MAX) {
    desiredPosition = ARM_POSITION_MAIN_MAX;
  }
    
  while (servoPositions.armMain < desiredPosition) {
    armTurnLeftWithoutEeprom();
    delay(5);
  }

  while (servoPositions.armMain > desiredPosition) {
    armTurnRightWithoutEeprom();
    delay(5);
  }

  EEPROM.put(0, servoPositions);
}

void armServoRightRotateToPosition(String caller) {
  if (servoPositions.armRight >= ARM_POSITION_RIGHT_MIN && servoPositions.armRight <= ARM_POSITION_RIGHT_MAX) {
    Serial.println(caller + ": servo right " + String(servoPositions.armRight));
    armServoRight.write(servoPositions.armRight);
    EEPROM.put(0, servoPositions);
  }
}

void armServoRightRotateToPositionWithoutEeprom() {
  if (servoPositions.armRight >= ARM_POSITION_RIGHT_MIN && servoPositions.armRight <= ARM_POSITION_RIGHT_MAX) {
    armServoRight.write(servoPositions.armRight);
  }
}

void armServoLeftRotateToPosition(String caller) {
  if (servoPositions.armLeft >= ARM_POSITION_LEFT_MIN && servoPositions.armLeft <= ARM_POSITION_LEFT_MAX) {
    Serial.println(caller + ": servo left " + String(servoPositions.armLeft));
    armServoLeft.write(servoPositions.armLeft);
    EEPROM.put(0, servoPositions);
  }
}

void armServoLeftRotateToPositionWithoutEeprom() {
  if (servoPositions.armLeft >= ARM_POSITION_LEFT_MIN && servoPositions.armLeft <= ARM_POSITION_LEFT_MAX) {
    armServoLeft.write(servoPositions.armLeft);
  }
}

void armServoClawRotateToPosition(String caller) {
  if (servoPositions.armClaw >= CLAW_POSITION_MIN && servoPositions.armClaw <= CLAW_POSITION_MAX) {
    Serial.println(caller + ": servo claw " + String(servoPositions.armClaw));
    armServoClaw.write(servoPositions.armClaw);
    EEPROM.put(0, servoPositions);
  }
}

void armServoClawRotateToPositionWithoutEeprom() {
  if (servoPositions.armClaw >= CLAW_POSITION_MIN && servoPositions.armClaw <= CLAW_POSITION_MAX) {
    armServoClaw.write(servoPositions.armClaw);
  }
}

void armTurnRight() {
  servoPositions.armMain -= ARM_SERVOS_STEP;

  if (servoPositions.armMain < ARM_POSITION_MAIN_MIN) {
    servoPositions.armMain = ARM_POSITION_MAIN_MIN;
  }

  armServoMainRotateToPosition("armTurnRight");
}

void armTurnRightWithoutEeprom() {
  servoPositions.armMain -= ARM_SERVOS_STEP;

  if (servoPositions.armMain < ARM_POSITION_MAIN_MIN) {
    servoPositions.armMain = ARM_POSITION_MAIN_MIN;
  }

  armServoMainRotateToPositionWithoutEeprom();
}

void armTurnLeft() {
  servoPositions.armMain += ARM_SERVOS_STEP;

  if (servoPositions.armMain > ARM_POSITION_MAIN_MAX) {
    servoPositions.armMain = ARM_POSITION_MAIN_MAX;
  }

  armServoMainRotateToPosition("armTurnLeft");
}

void armTurnLeftWithoutEeprom() {
  servoPositions.armMain += ARM_SERVOS_STEP;

  if (servoPositions.armMain > ARM_POSITION_MAIN_MAX) {
    servoPositions.armMain = ARM_POSITION_MAIN_MAX;
  }

  armServoMainRotateToPositionWithoutEeprom();
}

void armTurnLeftMax() {
  while (servoPositions.armMain < ARM_POSITION_MAIN_MAX) {
    armTurnLeftWithoutEeprom();
    delay(5);
  }

  EEPROM.put(0, servoPositions);
}

void armTurnRightMax() {
  while (servoPositions.armMain > ARM_POSITION_MAIN_MIN) {
    armTurnRightWithoutEeprom();
    delay(5);
  }

  EEPROM.put(0, servoPositions);
}

void armTurnCenter() {
  while (servoPositions.armMain < ARM_POSITION_MAIN_DEFAULT) {
    armTurnLeft();
  }

  while (servoPositions.armMain > ARM_POSITION_MAIN_DEFAULT) {
    armTurnRight();
  }
}

void openClawWide() {
  while (servoPositions.armClaw < CLAW_POSITION_MAX) {
    servoPositions.armClaw++;
    armServoClawRotateToPositionWithoutEeprom();
    delay(10);
  }

  holdingPackage = false;
  
  EEPROM.put(0, servoPositions);
}

void openClawRelease() {
  while (servoPositions.armClaw < CLAW_POSITION_RELEASE) {
    servoPositions.armClaw++;
    armServoClawRotateToPositionWithoutEeprom();
    delay(10);
  }

  holdingPackage = false;
  
  EEPROM.put(0, servoPositions);
}

void closeClaw() {
  while (servoPositions.armClaw > CLAW_POSITION_MIN) {
    servoPositions.armClaw--;
    armServoClawRotateToPositionWithoutEeprom();
    delay(10);
  }
  
  EEPROM.put(0, servoPositions);
}

bool armConditionsCheck(int left, int right) {
  if (left < ARM_POSITION_LEFT_MIN) {
    armConditionsCheckWarning("left < ARM_POSITION_LEFT_MIN");
    
    return false;
  } else if (left > ARM_POSITION_LEFT_MAX) {
    armConditionsCheckWarning("left > ARM_POSITION_LEFT_MAX");
    
    return false;
  } else if (right < ARM_POSITION_RIGHT_MIN) {
    armConditionsCheckWarning("left < ARM_POSITION_RIGHT_MIN");
    
    return false;
  } else if (right > ARM_POSITION_RIGHT_MAX) {
    armConditionsCheckWarning("left > ARM_POSITION_RIGHT_MAX");
    
    return false;
  }

  return true;
}

void armConditionsCheckWarning(String warning) {
  Serial.println("Not allowed: " + warning);
  buzz(50);
}

// armPositionLeft: 0 - вытянута, 140 - втянута
void armForward() {
  int newLeft = servoPositions.armLeft - ARM_SERVOS_STEP;

  if (armConditionsCheck(newLeft, servoPositions.armRight)) {
    servoPositions.armLeft = newLeft;
    armServoLeftRotateToPosition("armForward");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armBack() {
  int newLeft = servoPositions.armLeft + ARM_SERVOS_STEP;
  
  if (armConditionsCheck(newLeft, servoPositions.armRight)) {
    servoPositions.armLeft = newLeft;
    armServoLeftRotateToPosition("armBack");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  int newRight = servoPositions.armRight - ARM_SERVOS_STEP;

  if (armConditionsCheck(servoPositions.armLeft, newRight)) {
    servoPositions.armRight = newRight;
    armServoRightRotateToPosition("armUp");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armDown() {
  int newRight = servoPositions.armRight + ARM_SERVOS_STEP;
  
  if (armConditionsCheck(servoPositions.armLeft, newRight)) {
    servoPositions.armRight = newRight;
    armServoRightRotateToPosition("armDown");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armToDefaultPosition() {
  armToPosition(ARM_POSITION_LEFT_DEFAULT, ARM_POSITION_RIGHT_DEFAULT, 10);
}

void armToPosition(int left, int right, int delayBetween) {
  while (servoPositions.armRight != right || servoPositions.armLeft != left) {
    if (servoPositions.armRight > right) {
      servoPositions.armRight--;
    } else if (servoPositions.armRight < right) {
      servoPositions.armRight++;
    }

    if (servoPositions.armLeft < left) {
      servoPositions.armLeft++;
    } else if (servoPositions.armLeft > left) {
      servoPositions.armLeft--;
    }

    delay(delayBetween);
    armServoRightRotateToPositionWithoutEeprom();
    armServoLeftRotateToPositionWithoutEeprom();
  }

  EEPROM.put(0, servoPositions);
}


// <<< Arm functions
