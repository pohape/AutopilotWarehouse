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

int findObject() {
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
          delay(1000);
          degreePackageStart = servoPositions.armMain;
        } else {
          degreePackageEnd = servoPositions.armMain;
        }
  
        Serial.println(String(servoPositions.armMain) + ": " + String(distance));
      }
    }
  }

  int degreePackageCenter = degreePackageStart + (degreePackageEnd - degreePackageStart) / 2;
  Serial.println(String(degreePackageStart) + " - " + degreePackageCenter + " - " + String(degreePackageEnd));

  return degreePackageCenter;
}

void findObjectAndTurnThere() {
  int degreePackageCenter = findObject();
  armServoMainRotateSlowToPosition(degreePackageCenter);
}
