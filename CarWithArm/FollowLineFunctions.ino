// follow line functions >>>
void addMoveToLastMovesArray(int move) {
  if (lastFollowLineMoves[0] != move) { 
    lastFollowLineMoves[12] = lastFollowLineMoves[11];
    lastFollowLineMoves[11] = lastFollowLineMoves[10];
    lastFollowLineMoves[10] = lastFollowLineMoves[9];
    lastFollowLineMoves[9] = lastFollowLineMoves[8];
    lastFollowLineMoves[8] = lastFollowLineMoves[7];
    lastFollowLineMoves[7] = lastFollowLineMoves[6];
    lastFollowLineMoves[6] = lastFollowLineMoves[5];
    lastFollowLineMoves[5] = lastFollowLineMoves[4];
    lastFollowLineMoves[4] = lastFollowLineMoves[3];
    lastFollowLineMoves[3] = lastFollowLineMoves[2];
    lastFollowLineMoves[2] = lastFollowLineMoves[1];
    lastFollowLineMoves[1] = lastFollowLineMoves[0];
    lastFollowLineMoves[0] = move;
  }
}

void followLineCheckAndStop() {
  if (lastFollowLineMoves[0] != 0 && lastFollowLineMoves[1] != 0 && lastFollowLineMoves[2] != 0 && lastFollowLineMoves[3] != 0 && lastFollowLineMoves[4] != 0 && lastFollowLineMoves[5] != 0 && lastFollowLineMoves[6] != 0 && lastFollowLineMoves[7] != 0 && lastFollowLineMoves[8] != 0 && lastFollowLineMoves[9] != 0) {
    for(int i = 0; i < 13; i++)
    {
      Serial.println(lastFollowLineMoves[i]);
    }

    Serial.println();
  }

  if (lastFollowLineMoves[0] != 8 || lastFollowLineMoves[1] != 8 || lastFollowLineMoves[2] != 8 || lastFollowLineMoves[3] != 8 || lastFollowLineMoves[4] != 8 || lastFollowLineMoves[5] != 8 || lastFollowLineMoves[6] != 8 || lastFollowLineMoves[7] != 8 || lastFollowLineMoves[8] != 8 || lastFollowLineMoves[9] != 8 || lastFollowLineMoves[10] != 8 || lastFollowLineMoves[11] != 8 || lastFollowLineMoves[12] != 8) {
    int last = 0;
    int otherCount = 0;
    
    for (int i = 0; i < 13; i++)
    {
      if (last == lastFollowLineMoves[i]) {
        //Serial.println("BAD last == last");
        return;
      } else if (lastFollowLineMoves[i] != 2 && lastFollowLineMoves[i] != 8) {
        otherCount++;
    
        if (otherCount > 3) {
          //Serial.println("BAD otherCount == 3");
          return;
        }
      }
    
      last = lastFollowLineMoves[i];
    }
  }

  //Serial.println("Follow line STOP");
  mode = 1;
  lastFollowLineMoves[0] = 0;
  lastFollowLineMoves[1] = 0;
  lastFollowLineMoves[2] = 0;
  lastFollowLineMoves[3] = 0;
  lastFollowLineMoves[4] = 0;
  lastFollowLineMoves[5] = 0;
  lastFollowLineMoves[6] = 0;
  lastFollowLineMoves[7] = 0;
  lastFollowLineMoves[8] = 0;
  lastFollowLineMoves[9] = 0;
  lastFollowLineMoves[10] = 0;
  lastFollowLineMoves[11] = 0;
  lastFollowLineMoves[12] = 0;
}

void processFollowLine() {
  if (mode == 2) {
    processMode2();
  } else if (mode == 3) {
    processMode3();
  }
}

void mode3TurnRightFromObstruction() {
  do {
    leftForwardStart();
    delay(200);
    bothStop();
    distance = ultrasonic.Distance();

    if (distance < distanceWarning) {
      Serial.println("2. I SEE IT " + String(distance));
    } else {
      Serial.println("2. I DON'T SEE IT " + String(distance));
    }
  } while (distance < distanceWarning);

  Serial.println("mode3TurnRightFromObstruction done " + String(distance));
  delay(100);
}

bool mode3TurnUltrasonicLeftToObstruction() {
  do {
    armTurnLeft();
    distance = ultrasonic.Distance();

    if (distance < distanceWarning) {
      Serial.println("1. I SEE IT " + String(distance));
      return true;
    } else {
      Serial.println("1. I DON'T SEE IT " + String(distance));
    }
  } while (armPositionMain < armPositionMainMax);

  Serial.println("mode3TurnUltrasonicLeftToObstruction done " + String(distance));
  delay(100);
  
  return false;
}

void processMode3() {
  rightForwardStart();
  leftForwardStart();
  
  int center = digitalRead(PIN_TRACING_CENTER) == HIGH ? 1 : 0;
  int right = digitalRead(PIN_TRACING_RIGHT) == HIGH ? 1 : 0;
  int left = digitalRead(PIN_TRACING_LEFT) == HIGH ? 1 : 0;

  if ((center + right + left) >= 2) {
    bothStop();
    armTurnCenter();
    
    analogWrite(PIN_WHEELS_ENA, wheelsSpeedDefault);
    analogWrite(PIN_WHEELS_ENB, wheelsSpeedDefault);
    
    leftForwardStart();
    delay(700);

    rightForwardStart();
    delay(300);
    
    bothStop();
    mode = 2;
  }
}

void initMode3() {
  mode = 3;
  bothStop();

  do {
    mode3TurnRightFromObstruction();
  } while (mode3TurnUltrasonicLeftToObstruction());

  addMoveToLastMovesArray(1);
  rightForwardStart();
  leftForwardStart();
  delay(500);

  analogWrite(PIN_WHEELS_ENA, wheelsSpeedDefault - 30);
  analogWrite(PIN_WHEELS_ENB, wheelsSpeedDefault + 40);

  rightForwardStart();
  leftForwardStart();
}

void processMode2() {
  // найти черную линию
  // проехать вперед долю секунды

  int center = digitalRead(PIN_TRACING_CENTER);

  if (center == HIGH) {
    long distance = ultrasonic.Distance();
    
    if (distance < distanceWarning) {
      initMode3();
    } else {      
      addMoveToLastMovesArray(2);
      rightForwardStart();
      leftForwardStart();
    }
  } else {
    bothStop();

    int right = digitalRead(PIN_TRACING_RIGHT);
    int left = digitalRead(PIN_TRACING_LEFT);

    if (right == HIGH && left == LOW) {
      addMoveToLastMovesArray(1);
      leftForwardStart();
      delay(40);
    } else if (left == HIGH && right == LOW) {
      addMoveToLastMovesArray(3);
      rightForwardStart();
      delay(40);
    } else if (left == LOW && right == LOW) {
      addMoveToLastMovesArray(8);
      leftBackStart();
      rightBackStart();
      delay(50);
    } else {
      return;
    }
  }

  followLineCheckAndStop();
}
// <<< follow line functions
