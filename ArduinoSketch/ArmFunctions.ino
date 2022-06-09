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

void armServoRightRotateToPosition(String caller) {
  if (servoPositions.armRight >= ARM_POSITION_RIGHT_MIN && servoPositions.armRight <= ARM_POSITION_RIGHT_MAX) {
    Serial.println(caller + ": servo right " + String(servoPositions.armRight));
    armServoRight.write(servoPositions.armRight);
    EEPROM.put(0, servoPositions);
  }
}

void armServoLeftRotateToPosition(String caller) {
  if (servoPositions.armLeft >= ARM_POSITION_LEFT_MIN && servoPositions.armLeft <= ARM_POSITION_LEFT_MAX) {
    Serial.println(caller + ": servo left " + String(servoPositions.armLeft));
    armServoLeft.write(servoPositions.armLeft);
    EEPROM.put(0, servoPositions);
  }
}

void armServoClawRotateToPosition(String caller) {
  if (servoPositions.armClaw >= CLAW_POSITION_MIN && servoPositions.armClaw <= CLAW_POSITION_MAX) {
    Serial.println(caller + ": servo claw " + String(servoPositions.armClaw));
    armServoClaw.write(servoPositions.armClaw);
    EEPROM.put(0, servoPositions);
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

void openClaw() {
  servoPositions.armClaw = CLAW_POSITION_MAX;
  armServoClawRotateToPosition("openClaw");
}

void closeClaw() {
  servoPositions.armClaw = CLAW_POSITION_MIN;
  armServoClawRotateToPosition("closeClaw");
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
  } else if (left <= 20 && right < 40) {
    armConditionsCheckWarning("left <= 20 && right < 40");
    
    return false;
  } else if (left <= 30 && right < 30) {
    armConditionsCheckWarning("left <= 30 && right < 30");
    
    return false;
  } else if (left >= 150 && right > 30) {
    armConditionsCheckWarning("left >= 150 && right > 30");
    
    return false;
  } else if (left >= 100 && right >= 70) {
    armConditionsCheckWarning("left >= 100 && right >= 70");
    
    return false;
  } 
  else if (left >= 130 && left <= 150 && right > 35) {
    armConditionsCheckWarning("left >= 130 && left <= 150 && right > 35");
    
    return false;
  }
  else if (left >= 150 && right != 0) {
    armConditionsCheckWarning("left >= 150 && right != 0");

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
  // ARM_POSITION_RIGHT_MIN=0
  // ARM_POSITION_LEFT_MAX=170

  while (servoPositions.armRight > ARM_POSITION_RIGHT_MIN || servoPositions.armLeft < ARM_POSITION_LEFT_MAX) {
    if (servoPositions.armRight > ARM_POSITION_RIGHT_MIN) {
      servoPositions.armRight--;
      armServoRightRotateToPosition("armToDefaultPosition");
    }

    if (servoPositions.armLeft < ARM_POSITION_LEFT_MAX) {
      servoPositions.armLeft++;
      armServoLeftRotateToPosition("armToDefaultPosition");
    }
  }
}


// <<< Arm functions
