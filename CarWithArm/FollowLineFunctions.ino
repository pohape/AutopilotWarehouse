// follow line functions >>>
void addMoveToLastMovesArray(int move) {
  if (lastFollowLineMoves[0] != move) { 
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
  if ((lastFollowLineMoves[0] == 2 && lastFollowLineMoves[1] == 8 && lastFollowLineMoves[2] == 2 && lastFollowLineMoves[3] == 8 && lastFollowLineMoves[4] == 2 && lastFollowLineMoves[5] == 8 && lastFollowLineMoves[6] == 2 && lastFollowLineMoves[7] == 8 && lastFollowLineMoves[8] == 2 && lastFollowLineMoves[9] == 8 && lastFollowLineMoves[10] == 2 && lastFollowLineMoves[11] == 8 && lastFollowLineMoves[12] == 2 && lastFollowLineMoves[13] == 8 && lastFollowLineMoves[14] == 2) || (lastFollowLineMoves[0] == 8 && lastFollowLineMoves[1] == 2 && lastFollowLineMoves[2] == 8 && lastFollowLineMoves[3] == 2 && lastFollowLineMoves[4] == 8 && lastFollowLineMoves[5] == 2 && lastFollowLineMoves[6] == 8 && lastFollowLineMoves[7] == 2 && lastFollowLineMoves[8] == 8 && lastFollowLineMoves[9] == 2 && lastFollowLineMoves[10] == 8 && lastFollowLineMoves[11] == 2 && lastFollowLineMoves[12] == 8 && lastFollowLineMoves[13] == 2 && lastFollowLineMoves[14] == 8)) {
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
    lastFollowLineMoves[13] = 0;
    lastFollowLineMoves[14] = 0;
  }
}

void processFollowLine() {
  if (mode == 2) {
    // найти черную линию
    // проехать вперед долю секунды

    int center = digitalRead(PIN_TRACING_CENTER);

    if (center == HIGH) {
      addMoveToLastMovesArray(2);
      rightForwardStart();
      leftForwardStart();
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
}
// <<< follow line functions