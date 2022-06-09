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
}

bool findPackage() {
  //armToDefaultPosition();
  //armTurnRightMax();
  findClosestObjectAndTurnThere();

    
  
}

void findClosestObjectAndTurnThere() {
  armTurnRightMax();

  int closestObjectDegree = -1;
  int closestObjectDistance = 1000;

  for (servoPositions.armMain = ARM_POSITION_MAIN_MIN; servoPositions.armMain <= ARM_POSITION_MAIN_MAX; servoPositions.armMain++) {
    armServoMainRotateToPositionWithoutEeprom();
    distance = ultrasonic.Distance();

    if (distance < closestObjectDistance) {
      closestObjectDistance = distance;
      closestObjectDegree = servoPositions.armMain;
    }
  }

  while (servoPositions.armMain < closestObjectDegree) {
    armTurnLeftWithoutEeprom();
    delay(5);
  }

  while (servoPositions.armMain > closestObjectDegree) {
    armTurnRightWithoutEeprom();
    delay(5);
  }

  EEPROM.put(0, servoPositions);
}
