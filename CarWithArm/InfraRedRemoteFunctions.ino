void processIrButtons() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
//      Serial.println("Raw:");
//      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
//      Serial.println("Flags:");
//      Serial.println(IrReceiver.decodedIRData.flags);
//      Serial.println("printIRResultShort:");
//      IrReceiver.printIRResultShort( & Serial);

      if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == one) {
        remoteLastPushed = one;

        rightForwardStart();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == two) {
        remoteLastPushed = two;

        rightForwardStart();
        leftForwardStart();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == three) {
        remoteLastPushed = three;

        leftForwardStart();
      } else if (IrReceiver.decodedIRData.decodedRawData == five) {
        remoteLastPushed = five;

        if (mode == 1) {
          mode = 2;
        } else {
          mode = 1;
        }

        Serial.println("mode " + String(mode));
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == seven) {
        remoteLastPushed = seven;

        rightBackStart();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == eight) {
        remoteLastPushed = eight;

        rightBackStart();
        leftBackStart();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == nine) {
        remoteLastPushed = nine;

        leftBackStart();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == left) {
        remoteLastPushed = left;

        armTurnLeft();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == right) {
        remoteLastPushed = right;

        armTurnRight();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == up) {
        remoteLastPushed = up;

        armForward();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == down) {
        remoteLastPushed = down;

        armBack();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == star) {
        remoteLastPushed = star;

        armUp();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == grid) {
        remoteLastPushed = grid;

        armDown();
      } else if (mode == 1 && IrReceiver.decodedIRData.decodedRawData == ok) {
        remoteLastPushed = ok;

        if (servoPositions.armClaw == armPositionClawMin) {
          openClaw();
        } else {
          closeClaw();
        }
      }

      if (IrReceiver.decodedIRData.flags == IRDATA_FLAGS_IS_REPEAT) {
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
        } else if (remoteLastPushed == up) {
          armForward();
        } else if (remoteLastPushed == down) {
          armBack();
        } else if (remoteLastPushed == grid) {
          armDown();
        } else if (remoteLastPushed == star) {
          armUp();
        }
      }
    }
    
    infrared.resume(); // Receive the next value
  }
}
