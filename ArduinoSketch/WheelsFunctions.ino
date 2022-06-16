void manageStateOfWheels() {
  unsigned long currentMillis = millis();

  // if the wheel is moving more than $oneMoveMs ms
  if (leftCurrentlyMovingForward && rightCurrentlyMovingForward && (currentMillis - leftForwardStarted) > ONE_MOVE_BOTH_MS && (currentMillis - rightForwardStarted) > ONE_MOVE_BOTH_MS) {
    bothStop();
  } else if (leftCurrentlyMovingBack && rightCurrentlyMovingBack && (currentMillis - leftBackStarted) > ONE_MOVE_BOTH_MS && (currentMillis - rightBackStarted) > ONE_MOVE_BOTH_MS) {
    bothStop();
  } else if (leftCurrentlyMovingForward && (currentMillis - leftForwardStarted) > ONE_MOVE_SINGLE_MS) {
    //Serial.println(String(currentMillis) + ": leftCurrentlyMovingForward && leftForwardStarted " + String(leftForwardStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - leftForwardStarted));

    leftStop();
  } else if (rightCurrentlyMovingForward && (currentMillis - rightForwardStarted) > ONE_MOVE_SINGLE_MS) {
    //Serial.println(String(currentMillis) + ": rightCurrentlyMovingForward && rightForwardStarted " + String(rightForwardStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - rightForwardStarted));

    rightStop();
  } else if (leftCurrentlyMovingBack && (currentMillis - leftBackStarted) > ONE_MOVE_SINGLE_MS) {
    //Serial.println(String(currentMillis) + ": leftCurrentlyMovingBack && leftBackStarted " + String(leftBackStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - leftBackStarted));

    leftStop();
  } else if (rightCurrentlyMovingBack && (currentMillis - rightBackStarted) > ONE_MOVE_SINGLE_MS) {
    //Serial.println(String(currentMillis) + ": rightCurrentlyMovingBack && rightBackStarted " + String(rightBackStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - rightBackStarted));

    rightStop();
  }
}

void wheelsTurnAround() {
  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TURN_AROUND);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TURN_AROUND);

  digitalWrite(PIN_WHEELS_IN1, HIGH);
  digitalWrite(PIN_WHEELS_IN2, LOW);
  digitalWrite(PIN_WHEELS_IN3, LOW);
  digitalWrite(PIN_WHEELS_IN4, HIGH);

  delay(1250);

  digitalWrite(PIN_WHEELS_IN1, HIGH);
  digitalWrite(PIN_WHEELS_IN2, LOW);
  digitalWrite(PIN_WHEELS_IN3, HIGH);
  digitalWrite(PIN_WHEELS_IN4, LOW);

  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);

  delay(300);

  digitalWrite(PIN_WHEELS_IN1, LOW);
  digitalWrite(PIN_WHEELS_IN2, LOW);
  digitalWrite(PIN_WHEELS_IN3, LOW);
  digitalWrite(PIN_WHEELS_IN4, LOW);
}

void bothForwardStart() {
  if (!rightCurrentlyMovingForward || !leftCurrentlyMovingForward) {
    analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
    analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    
    digitalWrite(PIN_WHEELS_IN1, HIGH);
    digitalWrite(PIN_WHEELS_IN2, LOW);
    digitalWrite(PIN_WHEELS_IN3, HIGH);
    digitalWrite(PIN_WHEELS_IN4, LOW);
  
//    delay(100);
  
    rightForwardStopped = 0;
    leftCurrentlyMovingForward = true;
    rightCurrentlyMovingForward = true;
    
    leftCurrentlyMovingBack = false;
    rightCurrentlyMovingBack = false;
  
//    analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TO_GO_BACK);
//    analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TO_GO_BACK);

    //Serial.println(String(currentMillis) + ": Both forward start");
  }

  unsigned long currentMillis = millis();
  rightForwardStarted = currentMillis;
  leftForwardStarted = currentMillis;
}

// Wheels left/right start/stop functions >>>
void rightForwardStart() {
  unsigned long currentMillis = millis();
  rightForwardStarted = currentMillis;

  if (!rightCurrentlyMovingForward) {
    rightForwardStopped = 0;
    rightCurrentlyMovingForward = true;
    rightCurrentlyMovingBack = false;
  
    digitalWrite(PIN_WHEELS_IN4, LOW);
    digitalWrite(PIN_WHEELS_IN3, HIGH);

    Serial.println(String(currentMillis) + ": Right forward start");
  }
}

void rightBackStart() {
  unsigned long currentMillis = millis();
  rightBackStarted = currentMillis;

  if (!rightCurrentlyMovingBack) {
    rightBackStopped = 0;
    rightCurrentlyMovingForward = false;
    rightCurrentlyMovingBack = true;
  
    digitalWrite(PIN_WHEELS_IN3, LOW);
    digitalWrite(PIN_WHEELS_IN4, HIGH);

    Serial.println(String(currentMillis) + ": Right back start");
  }
}

void leftForwardStart() {
  unsigned long currentMillis = millis();
  leftForwardStarted = currentMillis;

  if (!leftCurrentlyMovingForward) {
    leftForwardStopped = 0;
    leftCurrentlyMovingForward = true;
    leftCurrentlyMovingBack = false;
  
    digitalWrite(PIN_WHEELS_IN2, LOW);
    digitalWrite(PIN_WHEELS_IN1, HIGH);

    Serial.println(String(currentMillis) + ": Left forward start");
  }
}

void leftBackStart() {
  unsigned long currentMillis = millis();
  leftBackStarted = currentMillis;

  if (!leftCurrentlyMovingBack) {
    leftBackStopped = 0;
    leftCurrentlyMovingForward = false;
    leftCurrentlyMovingBack = true;
  
    digitalWrite(PIN_WHEELS_IN1, LOW);
    digitalWrite(PIN_WHEELS_IN2, HIGH);
  
    Serial.println(String(currentMillis) + ": Left back start");
  }
}

void leftStop() {
  if (leftCurrentlyMovingForward || leftCurrentlyMovingBack) {
    leftCurrentlyMovingForward = false;
    leftCurrentlyMovingBack = false;
  
    digitalWrite(PIN_WHEELS_IN1, LOW);
    digitalWrite(PIN_WHEELS_IN2, LOW);
  
    Serial.println(String(millis()) + ": Left stopped");
  }
}

void rightStop() {
  if (rightCurrentlyMovingForward || rightCurrentlyMovingBack) {
    rightCurrentlyMovingForward = false;
    rightCurrentlyMovingBack = false;
  
    digitalWrite(PIN_WHEELS_IN3, LOW);
    digitalWrite(PIN_WHEELS_IN4, LOW);
  
    Serial.println(String(millis()) + ": Right stopped");
  }
}

void bothStop() {
  if (leftCurrentlyMovingForward || leftCurrentlyMovingBack || rightCurrentlyMovingForward || rightCurrentlyMovingBack){
    digitalWrite(PIN_WHEELS_IN1, LOW);
    digitalWrite(PIN_WHEELS_IN2, LOW);
    digitalWrite(PIN_WHEELS_IN3, LOW);
    digitalWrite(PIN_WHEELS_IN4, LOW);

    leftCurrentlyMovingForward = false;
    leftCurrentlyMovingBack = false;
    rightCurrentlyMovingForward = false;
    rightCurrentlyMovingBack = false;

    Serial.println(String(millis()) + ": bothStop");
  }
}

// <<< Wheels left/right start/stop functions
