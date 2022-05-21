void processIrButtons() {
  if (infrared.decode( & infraredResults)) {
    remotePushed = String(infraredResults.value);

    Serial.println(remotePushed);

    if (mode == 1 && remotePushed == one) {
      remoteLastPushed = one;

      rightForwardStart();
    } else if (mode == 1 && remotePushed == two) {
      remoteLastPushed = two;

      rightForwardStart();
      leftForwardStart();
    } else if (mode == 1 && remotePushed == three) {
      remoteLastPushed = three;

      leftForwardStart();
    } else if (remotePushed == five) {
      remoteLastPushed = five;

      if (mode == 1) {
        mode = 2;
      } else {
        mode = 1;
      }

      Serial.println("mode " + String(mode));
    } else if (mode == 1 && remotePushed == seven) {
      remoteLastPushed = seven;

      rightBackStart();
    } else if (mode == 1 && remotePushed == eight) {
      remoteLastPushed = eight;

      rightBackStart();
      leftBackStart();
    } else if (mode == 1 && remotePushed == nine) {
      remoteLastPushed = nine;

      leftBackStart();
    } else if (mode == 1 && remotePushed == left) {
      remoteLastPushed = left;

      armTurnLeft();
    } else if (mode == 1 && remotePushed == right) {
      remoteLastPushed = right;

      armTurnRight();
    } else if (mode == 1 && remotePushed == up) {
      remoteLastPushed = up;

      armForward();
    } else if (mode == 1 && remotePushed == down) {
      remoteLastPushed = down;

      armBack();
    } else if (mode == 1 && remotePushed == star) {
      remoteLastPushed = star;

      armUp();
    } else if (mode == 1 && remotePushed == grid) {
      remoteLastPushed = grid;

      armDown();
    } else if (mode == 1 && remotePushed == ok) {
      remoteLastPushed = ok;

      if (servoPositions.armClaw == armPositionClawMin) {
        openClaw();
      } else {
        closeClaw();
      }
    }

    if (remotePushed == codeHold) {
      Serial.println(String(millis()) + ": hold " + remoteLastPushed);

      if (remoteLastPushed == two) {
        rightForwardStart();
        leftForwardStart();
      } else if (remoteLastPushed == eight) {
        rightBackStart();
        leftBackStart();
      } else if (remoteLastPushed == one) {
        rightForwardStart();
      } else if (remoteLastPushed == three) {
        leftForwardStart();
      } else if (remoteLastPushed == seven) {
        rightBackStart();
      } else if (remoteLastPushed == nine) {
        leftBackStart();
      } else if (remoteLastPushed == left) {
        armTurnLeft();
      } else if (remoteLastPushed == right) {
        armTurnRight();
      }
    } else {
      Serial.println(String(millis()) + ": pushed " + remoteLastPushed);
    }

    infrared.resume(); // Receive the next value
  }
}
