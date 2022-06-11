void processMode4() {
  
}

void findAndTakePackage() {
  // 1. Сложить руку в исходное состояние
  // 2. Вертеть влево и вправо, запомнить расстояние до ближайшего объекта на каждом градусе
  // 3. Повернуться в сторону ближайшего объекта
  // 4. Раскрыть клюв и начать приближать клюв к объекту, пока расстояние клюва до объекта не станет примерно равно CLAW_DISTANCE_HOLD
  // 5. Закрыть клюв
  // 6. Медленно поднимаем коробку и проверяем каждые несколько градусов поднятия, что коробка в клюве (CLAW_DISTANCE_HOLD)
  // 7. Если коробка выпала, то переключаемся в ручной режим и посылаем сигнал в Андроид, что нужна помощь оператора
  // 8. Если коробка не выпала, то разворачиваемся на 360 градусов и включаем режим слежения за линией

  armToDefaultPosition();
  armTurnRightMax();
  findObjectAndTurnThere();
  //  takePackage();
}

bool takePackage() {
  openClaw();

  for (int armPositionKey = 0; armPositionKey < ARM_TAKE_PACKAGE_POSITIONS_COUNT; armPositionKey++) {
    int left = ARM_TAKE_PACKAGE_POSITIONS[armPositionKey][0];
    int right = ARM_TAKE_PACKAGE_POSITIONS[armPositionKey][1];

    armToPosition(left, right);
    delay(100);
    clawDistance = analogRead(PIN_INFRARED_CLAW_DISTANCE);

    if (clawDistance <= CLAW_DISTANCE_HOLD) {
      closeClaw();
      delay(200);
      armToDefaultPosition();
      armTurnCenter();

      return true;
    }
  } 

  return false;
}

int findObjectRightToLeft() {
  int degreePackageStart = -1;
  int degreePackageEnd = -1;
  
  while (degreePackageStart == -1 || degreePackageEnd == -1) {
    armTurnRightMax();
    
    for (servoPositions.armMain = ARM_POSITION_MAIN_MIN; servoPositions.armMain <= ARM_POSITION_MAIN_MAX; servoPositions.armMain++) {
      armServoMainRotateToPositionWithoutEeprom();
      distance = ultrasonic.Distance();
  
      if (distance < MAX_DISTANCE_TO_PACKAGE) {
        buzz(1);

        if (degreePackageStart < 0) {
          buzz(500);
          degreePackageStart = servoPositions.armMain;
        } else {
          degreePackageEnd = servoPositions.armMain;
        }
      } else if (degreePackageEnd > 0) {
        buzz(500);
        break;
      }
    }
  }

  return degreePackageStart + (degreePackageEnd - degreePackageStart) / 2;
}

int findObjectLeftToRight() {
  int degreePackageStart = -1;
  int degreePackageEnd = -1;
  
  while (degreePackageStart == -1 || degreePackageEnd == -1) {
    armTurnLeftMax();
    
    for (servoPositions.armMain = ARM_POSITION_MAIN_MAX; servoPositions.armMain >= ARM_POSITION_MAIN_MIN; servoPositions.armMain--) {
      armServoMainRotateToPositionWithoutEeprom();
      distance = ultrasonic.Distance();
  
      if (distance < MAX_DISTANCE_TO_PACKAGE) {
        buzz(1);

        if (degreePackageEnd < 0) {
          buzz(500);
          degreePackageEnd = servoPositions.armMain;
        } else {
          degreePackageStart = servoPositions.armMain;
        }
      } else if (degreePackageStart > 0) {
        buzz(500);
        break;
      }
    }
  }

  return degreePackageStart + (degreePackageEnd - degreePackageStart) / 2;
}

int findObject() {
  int degreePackageCenter1 = findObjectRightToLeft();
  int degreePackageCenter2 = findObjectLeftToRight();
  int degreePackageCenter3 = findObjectRightToLeft();
  int degreePackageCenter4 = findObjectLeftToRight();
  int degreePackageCenter = (degreePackageCenter1 + degreePackageCenter2 + degreePackageCenter3 + degreePackageCenter4) / 4;
  Serial.println(String(degreePackageCenter1) + " - " + degreePackageCenter + " - " + String(degreePackageCenter2));

  return degreePackageCenter;
}

void findObjectAndTurnThere() {
  int degreePackageCenter = findObject();
  armServoMainRotateSlowToPosition(degreePackageCenter);
}
