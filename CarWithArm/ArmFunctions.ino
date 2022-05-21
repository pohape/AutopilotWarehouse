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
  servoPositions.armMain -= 5;

  if (servoPositions.armMain < armPositionMainMin) {
    servoPositions.armMain = armPositionMainMin;
  }

  armServoMainRotateToPosition("armTurnRight");
}

void armTurnLeft() {
  servoPositions.armMain += 5;

  if (servoPositions.armMain > armPositionMainMax) {
    servoPositions.armMain = armPositionMainMax;
  }

  armServoMainRotateToPosition("armTurnLeft");
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
  servoPositions.armLeft -= 5;
  
  if (servoPositions.armLeft < armPositionLeftMin) {
    servoPositions.armLeft = armPositionLeftMin;
  }

  armServoLeftRotateToPosition("armForward");

//  if (!armConditionsCheck()) {
//    break;
//  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armBack() {
  servoPositions.armLeft += 5;
  
  if (servoPositions.armLeft > armPositionLeftMax) {
    servoPositions.armLeft = armPositionLeftMax;
  }

  armServoLeftRotateToPosition("armBack");

//  if (!armConditionsCheck()) {
//    break;
//  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

// armPositionRight: 20 - поднята, 80 - опущена
void armUp() {
  servoPositions.armRight -= 5;
  
  if (servoPositions.armRight < armPositionRightMin) {
    servoPositions.armRight = armPositionRightMin;
  }

  armServoRightRotateToPosition("armUp");
  
//  if (!armConditionsCheck()) {
//    break;
//  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}

void armDown() {
  servoPositions.armRight += 5;
  
  armServoRightRotateToPosition("armDown");

//  if (!armConditionsCheck()) {
//    break;
//  }

  Serial.println("Left: " + String(servoPositions.armLeft) + "; Right: " + String(servoPositions.armRight));
}


// <<< Arm functions
