// follow line functions >>>
void addMoveToLastMovesArray(int move) {
  if (move == 8) {
    backInRowCount++;
  } else {
    backInRowCount = 0;
  }
  
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
  if (backInRowCount < 30) {
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

  setMode(1);
  
  backInRowCount = 0;
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
    analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
    analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    leftForwardStart();
    
    delay(300);
    bothStop();
    
    updateDistanceCm();

    if (distance < DISTANCE_WARNING) {
      Serial.println("2. I SEE IT " + String(distance));
    } else {
      Serial.println("2. I DON'T SEE IT " + String(distance));
    }
  } while (distance < DISTANCE_WARNING);

  Serial.println("mode3TurnRightFromObstruction done " + String(distance));
  delay(100);
}

bool mode3TurnUltrasonicLeftToObstruction() {
  do {
    armTurnLeft();
    updateDistanceCm();

    if (distance < DISTANCE_WARNING) {
      Serial.println("1. I SEE IT " + String(distance));
      return true;
    } 
    else {
      Serial.println("1. I DON'T SEE IT " + String(distance));
    }
  } while (servoPositions.armMain < ARM_POSITION_MAIN_MAX);

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
    
    analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
    analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    
    leftForwardStart();
    delay(700);

    rightForwardStart();
    delay(300);
    
    bothStop();
    setMode(2);
  }
}

void initMode3() {
  bothStop();
  setMode(3);
  
  rightBackStart();
  leftBackStart();
  delay(300);
  bothStop();

  do {
    mode3TurnRightFromObstruction();
  } while (mode3TurnUltrasonicLeftToObstruction());

  addMoveToLastMovesArray(1);
  rightForwardStart();
  leftForwardStart();
  delay(500);

  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT - 30);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT + 40);

  rightForwardStart();
  leftForwardStart();
}

void processMode2() {
  // найти черную линию
  // проехать вперед долю секунды

  int center = digitalRead(PIN_TRACING_CENTER);

  if (center == HIGH) {
    updateDistanceCm();
    
    if (distance < DISTANCE_WARNING) {
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
      delay(30);
    } else {
      return;
    }
  }

  followLineCheckAndStop();
}

void updateDistanceCm() {
  int distanceNew = ultrasonic.Distance();
  int distanceMin = distance * 0.9;
  int distanceMax = distance * 1.1;
  
  if (distanceNew >= distanceMin && distanceNew <= distanceMax) {
    distance = distanceNew;
  } else {
    delay(5);
    int distanceNew2 = ultrasonic.Distance();
    //Serial.println("distance try 2, old was " + String(distance) + "; new is " + String(distanceNew) + " and newest is " + String(distanceNew2));
  
    if (distanceNew >= distanceMin && distanceNew <= distanceMax) {
      distance = distanceNew2;
    } else {
      delay(5);
      int distanceNew3 = ultrasonic.Distance();
      //Serial.println("distance try 3: " + String(distanceNew3));

      float diff1 = float(distanceNew) / float(distance) - 1;
      float diff2 = float(distanceNew2) / float(distance) - 1;
      float diff3 = float(distanceNew3) / float(distance) - 1;

      diff1 = diff1 < 0 ? diff1 * -1 : diff1;
      diff2 = diff2 < 0 ? diff2 * -1 : diff2;
      diff3 = diff3 < 0 ? diff3 * -1 : diff3;

      if (diff1 <= diff2 && diff1 <= diff3) {
        distance = distanceNew;
      } else if (diff2 <= diff1 && diff2 <= diff3) {
        distance = distanceNew2;
      } else {
        distance = distanceNew3;
      }
    }
  }
}
// <<< follow line functions
