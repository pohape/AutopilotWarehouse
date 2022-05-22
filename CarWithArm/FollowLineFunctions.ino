// follow line functions >>>
void addMoveToLastMovesArray(int move) {
  if (move == 8) {
    backInRowCount++;
  } else {
    backInRowCount = 0;
  }
  
  if (lastFollowLineMoves[0] != move) { 
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
  if (backInRowCount < MAX_BACK_IN_ROW_TO_STOP) {
    return;
//    int last = 0;
//    int otherCount = 0;
//    
//    for (int i = 0; i < 16; i++)
//    {
//      if (last == lastFollowLineMoves[i]) {
//        //Serial.println("BAD last == last");
//        return;
//      } else if (lastFollowLineMoves[i] != 2 && lastFollowLineMoves[i] != 8) {
//        otherCount++;
//    
//        if (otherCount > 3) {
//          //Serial.println("BAD otherCount == 3");
//          return;
//        }
//      }
//    
//      last = lastFollowLineMoves[i];
//    }
  }

//  for (int i = 0; i < 16; i++)
//  {
//    Serial.println(String(i) + ": " + String(lastFollowLineMoves[i]));
//  }

  setMode(1, "followLineCheckAndStop "+ String(backInRowCount));
  
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
  lastFollowLineMoves[13] = 0;
  lastFollowLineMoves[14] = 0;
  lastFollowLineMoves[15] = 0;
}

void processFollowLine() {
  if (mode == 2) {
    processMode2();
  } else if (mode == 3) {
    processMode3();
  }
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
    Serial.println("Lost center, both stop");
    bothStop();

    int right = digitalRead(PIN_TRACING_RIGHT);
    int left = digitalRead(PIN_TRACING_LEFT);

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

      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_TO_GO_BACK);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_TO_GO_BACK);
      
      leftBackStart();
      rightBackStart();
      delay(SEARCH_LINE_BACK_DELAY);

      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
    } else {
      // TODO: process this case
      Serial.println("Both BLACK and center is NO, do nothing");
      Serial.println("Left " + String(left));
      Serial.println("Center " + String(center));
      Serial.println("Right " + String(right));
      
      return;
    }
  }

  followLineCheckAndStop();
}
// <<< follow line functions
