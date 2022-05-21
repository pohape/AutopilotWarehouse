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

  if (servoPositionsFromEeprom.armClaw >= armPositionClawMin && servoPositionsFromEeprom.armClaw <= armPositionClawMax) {
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
  if (servoPositions.armClaw >= armPositionClawMin && servoPositions.armClaw <= armPositionClawMax) {
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

void armTurnLeft() {
  servoPositions.armMain += ARM_SERVOS_STEP;

  if (servoPositions.armMain > ARM_POSITION_MAIN_MAX) {
    servoPositions.armMain = ARM_POSITION_MAIN_MAX;
  }

  armServoMainRotateToPosition("armTurnLeft");
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
  servoPositions.armClaw = armPositionClawMax;
  armServoClawRotateToPosition("openClaw");
}

void closeClaw() {
  servoPositions.armClaw = armPositionClawMin;
  armServoClawRotateToPosition("closeClaw");
}

bool armConditionsCheck(int left, int right) {
  if (left <= 20 && right < 40) {
    Serial.println("left <= 20 && right < 40");
    buzzWarning();
    
    return false;
  } else if (left <= 30 && right < 30) {
    Serial.println("left <= 30 && right < 30");
    buzzWarning();
    
    return false;
  } else if (left >= 150 && right > 30) {
    Serial.println("left >= 150 && right > 30");
    buzzWarning();
    
    return false;
  } else if (left >= 100 && right >= 70) {
    Serial.println("left >= 150 && right > 30");
    buzzWarning();
    
    return false;
  } 
  else if (left >= 130 && left <= 150 && right > 35) {
    Serial.println("left >= 130 && left <= 150 && right > 35");
    buzzWarning();
    
    return false;
  }
  else if (left >= 150 && right != 0) {
    Serial.println("left >= 150 && right != 0");
    buzzWarning();
    
    return false;
  }

  return true;
}

void buzzWarning() {
  //tone(PIN_BUZZER, 550, 100);

//  buzzer.begin(0);
//
//  buzzer.distortion(NOTE_C3, NOTE_C5);
//  buzzer.distortion(NOTE_C5, NOTE_C3);
//
//  buzzer.end(1000);

//  tone(PIN_BUZZER, 1000); // Send 1KHz sound signal...
//  delay(1000);        // ...for 1 sec
//  noTone(PIN_BUZZER);     // Stop sound...
}

// armPositionLeft: 0 - вытянута, 140 - втянута
void armForward() {
  int newLeft = servoPositions.armLeft - ARM_SERVOS_STEP;

  if (newLeft < ARM_POSITION_LEFT_MIN) {
    newLeft = ARM_POSITION_LEFT_MIN;
  }

  if (armConditionsCheck(newLeft, servoPositions.armRight)) {
    servoPositions.armLeft = newLeft;
    armServoLeftRotateToPosition("armForward");
  } else {
    Serial.println("Not allowed!");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armBack() {
  int newLeft = servoPositions.armLeft + ARM_SERVOS_STEP;

  if (newLeft > ARM_POSITION_LEFT_MAX) {
    newLeft = ARM_POSITION_LEFT_MAX;
  }
  
  if (armConditionsCheck(newLeft, servoPositions.armRight)) {
    servoPositions.armLeft = newLeft;
    armServoLeftRotateToPosition("armBack");
  } else {
    Serial.println("Not allowed!");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  
  int newRight = servoPositions.armRight - ARM_SERVOS_STEP;
  Serial.println("armUp");
  Serial.println("old right: " + String(servoPositions.armRight));
  Serial.println("new right: " + String(newRight));

  if (newRight < ARM_POSITION_RIGHT_MIN) {
    newRight = ARM_POSITION_RIGHT_MIN;
  }

  Serial.println("new right2: " + String(newRight));

  if (armConditionsCheck(servoPositions.armLeft, newRight)) {
    servoPositions.armRight = newRight;
    armServoRightRotateToPosition("armUp");
  } else {
    Serial.println("Not allowed!");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armDown() {
  int newRight = servoPositions.armRight + ARM_SERVOS_STEP;

  Serial.println("armUp");
  Serial.println("old right: " + String(servoPositions.armRight));
  Serial.println("new right: " + String(newRight));
  
  if (newRight > ARM_POSITION_RIGHT_MAX) {
    newRight = ARM_POSITION_RIGHT_MAX;
  }

  Serial.println("new right2: " + String(newRight));
  
  if (armConditionsCheck(servoPositions.armLeft, newRight)) {
    servoPositions.armRight = newRight;
    armServoRightRotateToPosition("armDown");
  } else {
    Serial.println("Not allowed!");
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}


// <<< Arm functions
