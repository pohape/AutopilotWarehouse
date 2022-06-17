// follow line functions >>>
void addMoveToLastMovesArray(int move) {
  if (lastFollowLineMoves[0] != move) { 
    processBluetooth();

    lastFollowLineMoves[15] = lastFollowLineMoves[14];
    lastFollowLineMoves[14] = lastFollowLineMoves[13];
    lastFollowLineMoves[13] = lastFollowLineMoves[12];
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
// WARNING: THIS println CODE SLOWS DOWN THE OTHER CODE AND THE FOLLOW LINE FUNCTION WORKS NOT AS EXPECTED!
//  Serial.println();
//
//  for (int i = 0; i < 16; i++)
//  {
//    Serial.println(String(i) + ": " + String(lastFollowLineMoves[i]));
//  }
//
//  Serial.println();
  
  int last = 0;
  int otherCount = 0;
  
  for (int i = 0; i < 16; i++)
  {
    if (last == lastFollowLineMoves[i]) {
      //Serial.println("BAD last == last");
      return;
    } else if (lastFollowLineMoves[i] != 2 && lastFollowLineMoves[i] != 8) {
      otherCount++;
  
      if (otherCount > 2) {
        //Serial.println("BAD otherCount == 2");
        return;
      }
    }
  
    last = lastFollowLineMoves[i];
  }

  setMode(MODE_MANUAL, SWITCH_MODE_REASON_LINE_ENDED);

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
  lastFollowLineMoves[13] = 0;
  lastFollowLineMoves[14] = 0;
  lastFollowLineMoves[15] = 0;
}

void processMode2() {
  // найти черную линию
  // проехать вперед долю секунды

  int center = digitalRead(PIN_TRACING_CENTER);
  int right = digitalRead(PIN_TRACING_RIGHT);
  int left = digitalRead(PIN_TRACING_LEFT);

  if (center == HIGH) {
    if (left == HIGH && right == HIGH) {
      allHighInRowCount++;

      if (allHighInRowCount >= ALL_HIGH_IN_ROW_COUNT_TO_STOP) {
        bothStop();
        allHighInRowCount = 0;
        setMode(MODE_MANUAL, SWITCH_MODE_REASON_LINE_ENDED);
        
        return;
      }
    } else {
      allHighInRowCount = 0;
    }

    if (obstacles == 0 && (followLineStarted + 5000) < millis()) {
      updateDistanceCm();
    } else {
      distance = 100;
      Serial.println("distance 100");
    }
    
    if (distance < DISTANCE_WARNING) {
      obstacles++;
      initMode3();
    } else {
      addMoveToLastMovesArray(2);
      bothForwardStart();
    }
  } else {
    Serial.println("Lost center, both stop");
    bothStop();

    if (right == HIGH && left == LOW) {
      Serial.println("Left BLACK, right NO, call leftForwardStart()");
      addMoveToLastMovesArray(1);

      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TO_TURN);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TO_TURN);
      
      leftForwardStart();
      delay(ONE_WHEEL_TURN_DELAY);

      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    } else if (left == HIGH && right == LOW) {
      Serial.println("Left NO, right BLACK, call rightForwardStart()");
      addMoveToLastMovesArray(3);
      
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TO_TURN);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TO_TURN);
      
      rightForwardStart();
      delay(ONE_WHEEL_TURN_DELAY);

      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    } else if (left == LOW && right == LOW) {
      Serial.println("Both NO, drive back");
      addMoveToLastMovesArray(8);
      findLineBackwards();
    }
  }

  followLineCheckAndStop();
}

void findLineBackwards() {
  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
  
  digitalWrite(PIN_WHEELS_IN1, LOW);
  digitalWrite(PIN_WHEELS_IN2, HIGH);
  digitalWrite(PIN_WHEELS_IN3, LOW);
  digitalWrite(PIN_WHEELS_IN4, HIGH);

  delay(50);

  int right = digitalRead(PIN_TRACING_RIGHT) ? 1 : 0;
  int center = digitalRead(PIN_TRACING_CENTER) ? 1 : 0;
  int left = digitalRead(PIN_TRACING_LEFT) ? 1 : 0;
  int sum = right + center + left;

  if (sum == 0) {
    delay(50);

    int right = digitalRead(PIN_TRACING_RIGHT) ? 1 : 0;
    int center = digitalRead(PIN_TRACING_CENTER) ? 1 : 0;
    int left = digitalRead(PIN_TRACING_LEFT) ? 1 : 0;
    int sum = right + center + left;
  
    if (sum == 0) {
      delay(50);
  
      int right = digitalRead(PIN_TRACING_RIGHT) ? 1 : 0;
      int center = digitalRead(PIN_TRACING_CENTER) ? 1 : 0;
      int left = digitalRead(PIN_TRACING_LEFT) ? 1 : 0;
      int sum = right + center + left;
    
      if (sum == 0) {
        delay(50);
      
        analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TO_GO_BACK);
        analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TO_GO_BACK);
      
        right = digitalRead(PIN_TRACING_RIGHT) ? 1 : 0;
        center = digitalRead(PIN_TRACING_CENTER) ? 1 : 0;
        left = digitalRead(PIN_TRACING_LEFT) ? 1 : 0;
        sum = right + center + left;
      
        if (sum == 0) {
          int backInRowCount = 0;
          
          do {
            if (backInRowCount > MAX_BACK_IN_ROW_TO_STOP) {
              digitalWrite(PIN_WHEELS_IN1, LOW);
              digitalWrite(PIN_WHEELS_IN2, LOW);
              digitalWrite(PIN_WHEELS_IN3, LOW);
              digitalWrite(PIN_WHEELS_IN4, LOW);
              setMode(MODE_MANUAL, SWITCH_MODE_REASON_LINE_LOST);

              return;
            }

            backInRowCount++;
            delay(1);
            
            right = digitalRead(PIN_TRACING_RIGHT) ? 1 : 0;
            center = digitalRead(PIN_TRACING_CENTER) ? 1 : 0;
            left = digitalRead(PIN_TRACING_LEFT) ? 1 : 0;
            sum = right + center + left;
          } while (sum == 0);
        }
      }
    }
  }

  digitalWrite(PIN_WHEELS_IN1, LOW);
  digitalWrite(PIN_WHEELS_IN2, LOW);
  digitalWrite(PIN_WHEELS_IN3, LOW);
  digitalWrite(PIN_WHEELS_IN4, LOW);
  
  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
}
// <<< follow line functions
