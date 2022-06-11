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
  int degreePackageStart1 = -1;
  int degreePackageEnd1 = -1;
  int degreePackageStart2 = -1;
  int degreePackageEnd2 = -1;


  while (degreePackageStart1 == -1 || degreePackageEnd1 == -1) {
    armTurnRightMax();
    
    for (servoPositions.armMain = ARM_POSITION_MAIN_MIN; servoPositions.armMain <= ARM_POSITION_MAIN_MAX; servoPositions.armMain++) {
      armServoMainRotateToPositionWithoutEeprom();
      distance = ultrasonic.Distance();
  
      if (distance < MAX_DISTANCE_TO_PACKAGE) {
        buzz(1);

        if (degreePackageStart1 < 0) {
          buzz(500);
          degreePackageStart1 = servoPositions.armMain;
        } else {
          degreePackageEnd1 = servoPositions.armMain;
        }
  
        Serial.println(String(servoPositions.armMain) + ": " + String(distance));
      } else if (degreePackageEnd1 > 0) {
        buzz(500);
        break;
      }
    }
  }

   while (degreePackageStart2 == -1 || degreePackageEnd2 == -1 || (degreePackageEnd2 - degreePackageStart2) < 10) {
    armTurnLeftMax();
    
    for (servoPositions.armMain = ARM_POSITION_MAIN_MAX; servoPositions.armMain >= ARM_POSITION_MAIN_MIN; servoPositions.armMain--) {
      armServoMainRotateToPositionWithoutEeprom();
      distance = ultrasonic.Distance();
  
      if (distance < MAX_DISTANCE_TO_PACKAGE) {
        buzz(1);

        if (degreePackageEnd2 < 0) {
          buzz(500);
          degreePackageEnd2 = servoPositions.armMain;
        } else {
          degreePackageStart2 = servoPositions.armMain;
        }
  
        Serial.println(String(servoPositions.armMain) + ": " + String(distance));
      } else if (degreePackageStart2 > 0) {
        buzz(500);
        break;
      }
    }
  }

  int degreePackageCenter1 = degreePackageStart1 + (degreePackageEnd1 - degreePackageStart1) / 2;
  int degreePackageCenter2 = degreePackageStart2 + (degreePackageEnd2 - degreePackageStart2) / 2;
  int degreePackageCenter = (degreePackageCenter1 + degreePackageCenter2) / 2;
  Serial.println(String(degreePackageStart1) + " - " + degreePackageCenter1 + " - " + String(degreePackageEnd1));
  Serial.println(String(degreePackageStart2) + " - " + degreePackageCenter2 + " - " + String(degreePackageEnd2));
  Serial.println(degreePackageCenter);


  return degreePackageCenter;
}

void findObjectAndTurnThere() {
  int degreePackageCenter = findObject();
  armServoMainRotateSlowToPosition(degreePackageCenter);
}
