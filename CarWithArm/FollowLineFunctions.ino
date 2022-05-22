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
// <<< follow line functions
