void findShelfAndLeavePackage() {
  closeClaw();
  
  if (findObjectAndTurnThere() == PACKAGE_NOT_FOUND_BY_ULTRASONIC) {
    setMode(MODE_MANUAL, SWITCH_MODE_REASON_PACKAGE_NOT_FOUND);
  } else {
    leavePackageOnTop();
  }
}


void leavePackageOnTop() {
  int distance1 = ultrasonic.Distance();
  delay(60);

  servoPositions.armMain += 4;
  armServoMainRotateToPositionWithoutEeprom();
  int distance2 = ultrasonic.Distance();
  delay(60);

  servoPositions.armMain -= 8;
  armServoMainRotateToPositionWithoutEeprom();
  int distance3 = ultrasonic.Distance();
  delay(60);

  servoPositions.armMain += 4;
  armServoMainRotateToPositionWithoutEeprom();
  int distance4 = ultrasonic.Distance();

  int total = 0;
  int count = 0;

  if (distance1 <= 20) {
    total += distance1;
    count++;
  }
  
  if (distance2 <= 20) {
    total += distance2;
    count++;
  }
  
  if (distance3 <= 20) {
    total += distance3;
    count++;
  }
  
  if (distance4 <= 20) {
    total += distance4;
    count++;
  }

  int resultDistance = 20;

  if (count > 0) {
    resultDistance = total / count;
  }

  int left;
  int right;

  if (resultDistance <= 7) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[0][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[0][1];
  } else if (resultDistance == 8) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[1][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[1][1];
  } else if (resultDistance >= 9 && resultDistance <= 10) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[2][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[2][1];
  } else if (resultDistance == 11 || resultDistance == 12) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[3][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[3][1];
  } else if (resultDistance == 13) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[4][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[4][1];
  } else if (resultDistance >= 14) {
    left = ARM_LEAVE_PACKAGE_TOP_POSITIONS[5][0];
    right = ARM_LEAVE_PACKAGE_TOP_POSITIONS[5][1];
  }

//  if (resultDistance <= 7) {
//    left = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[0][0];
//    right = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[0][1];
//  } else if (resultDistance >= 8 && resultDistance <= 11) {
//    left = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[1][0];
//    right = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[1][1];
//  } else if (resultDistance >= 12 && resultDistance <= 13) {
//    left = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[2][0];
//    right = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[2][1];
//  } else if (resultDistance >= 14) {
//    left = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[3][0];
//    right = ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[3][1];
//  }

  armToPosition(left, right, 10);

  delay(500);
  openClawRelease();
  delay(500);
  armToDefaultPosition();
  setMode(MODE_MANUAL, SWITCH_MODE_REASON_RELEASED_PACKAGE);
}
