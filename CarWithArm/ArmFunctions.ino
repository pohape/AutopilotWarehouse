// Arm functions >>>

void initializeArm() {
  ServoPositions servoPositionsFromEeprom = {-1, -1, -1};
  EEPROM.get(0, servoPositionsFromEeprom);

  if (servoPositionsFromEeprom.armMain >= armPositionMainMin && servoPositionsFromEeprom.armMain <= armPositionMainMax) {
    servoPositions.armMain = servoPositionsFromEeprom.armMain;
  }

  armServoMain.attach(PIN_ARM_MAIN);
  armServoMainRotateToPosition("initialize main");
      
  if (servoPositionsFromEeprom.armRight >= armPositionRightMin && servoPositionsFromEeprom.armRight <= armPositionRightMax) {
    servoPositions.armRight = servoPositionsFromEeprom.armRight;
  }

  armServoRight.attach(PIN_ARM_RIGHT);
  armServoRightRotateToPosition("initialize right");

  if (servoPositionsFromEeprom.armLeft >= armPositionLeftMin && servoPositionsFromEeprom.armLeft <= armPositionLeftMax) {
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
  if (servoPositions.armMain >= armPositionMainMin && servoPositions.armMain <= armPositionMainMax) {
    Serial.println(caller + ": servo main " + String(servoPositions.armMain));
    armServoMain.write(servoPositions.armMain);
    EEPROM.put(0, servoPositions);
  }
}

void armServoRightRotateToPosition(String caller) {
  if (servoPositions.armRight >= armPositionRightMin && servoPositions.armRight <= armPositionRightMax) {
    Serial.println(caller + ": servo right " + String(servoPositions.armRight));
    armServoRight.write(servoPositions.armRight);
    EEPROM.put(0, servoPositions);
  }
}

void armServoLeftRotateToPosition(String caller) {
  if (servoPositions.armLeft >= armPositionLeftMin && servoPositions.armLeft <= armPositionLeftMax) {
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
  //Serial.println("Turn right\n");

  int targetPos = servoPositions.armMain - 10;

  if (targetPos < armPositionMainMin) {
    targetPos = armPositionMainMin;
  }

  for (servoPositions.armMain; servoPositions.armMain > targetPos; servoPositions.armMain--) {
    armServoMainRotateToPosition("armTurnRight");
    delay(5); // delay 5ms（used to adjust the servo speed）
  }
}

void armTurnLeft() {
  //Serial.println("Turn left\n");

  int targetPos = servoPositions.armMain + 10;

  if (targetPos > armPositionMainMax) {
    targetPos = armPositionMainMax;
  }

  for (servoPositions.armMain; servoPositions.armMain < targetPos; servoPositions.armMain++) {
    armServoMainRotateToPosition("armTurnLeft");
    delay(5);
  }
}

void armTurnCenter() {
  while (servoPositions.armMain < armPositionMainDefault) {
    armTurnLeft();
  }

  while (servoPositions.armMain > armPositionMainDefault) {
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

bool armConditionsCheck() {
  return true;
//  if (servoPositions.armLeft <= 20 && servoPositions.servoPositions.armRight < 40) {
//    Serial.println("armPositionLeft <= 20 && armPositionRight < 40");
//    
//    return false;
//  } else if (servoPositions.armLeft <= 30 && servoPositions.armRight < 30) {
//    Serial.println("armPositionLeft <= 30 && armPositionRight < 30");
//    
//    return false;
//  } else if (servoPositions.armLeft >= 150 && servoPositions.armRight > 30) {
//    Serial.println("armPositionLeft >= 150 && armPositionRight > 30");
//    
//    return false;
//  } 
////  else if (armPositionLeft >= 130 && armPositionLeft <= 150 && armPositionRight > 50) {
////    Serial.println("armPositionLeft >= 130 && armPositionLeft <= 150 && armPositionRight > 50");
////    
////    return false;
////  }
//  else if (armPositionLeft >= 150 && armPositionRight != 0) {
//    Serial.println("armPositionLeft >= 150 && armPositionRight != 0");
//    
//    return false;
//  }
//
//  return true;
}

// armPositionLeft: 0 - вытянута, 140 - втянута
void armForward() {
  int targetPos = servoPositions.armLeft - 10;
  
  if (targetPos < armPositionLeftMin) {
    targetPos = armPositionLeftMin;
  }

  for (servoPositions.armLeft; servoPositions.armLeft > targetPos; servoPositions.armLeft--) {
    armServoLeftRotateToPosition("armForward");
    delay(5);

    if (!armConditionsCheck()) {
      break;
    }
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armBack() {
  int targetPos = servoPositions.armLeft + 10;
  
  if (targetPos > armPositionLeftMax) {
    targetPos = armPositionLeftMax;
  }

  for (servoPositions.armLeft; servoPositions.armLeft < targetPos; servoPositions.armLeft++) {
    armServoLeftRotateToPosition("armBack");
    delay(5);

    if (!armConditionsCheck()) {
      break;
    }
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  int targetPos = servoPositions.armRight - 10;
  
  if (targetPos < armPositionRightMin) {
    targetPos = armPositionRightMin;
  }

  for (servoPositions.armRight; servoPositions.armRight > targetPos; servoPositions.armRight--) {
    armServoRightRotateToPosition("armUp");
    delay(5);
    
    if (!armConditionsCheck()) {
      break;
    }
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armDown() {
  int targetPos = servoPositions.armRight + 10;
  
  if (targetPos > armPositionRightMax) {
    targetPos = armPositionRightMax;
  }

  for (servoPositions.armRight; servoPositions.armRight < targetPos; servoPositions.armRight++) {
    armServoRightRotateToPosition("armDown");
    delay(5);

    if (!armConditionsCheck()) {
      break;
    }
  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}


// <<< Arm functions
