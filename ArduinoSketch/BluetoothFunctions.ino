void btSetMode(int newMode, int reason) {
  if (newMode == 1) {
    btSerialWrite('1');
  } else if (newMode == 2) {
    btSerialWrite('2');
  } else if (newMode == 3) {
    btSerialWrite('3');
  } else if (newMode == 4) {
    btSerialWrite('4');
  }

  if (reason == 1) {
    btSerialWrite('1');
  } else if (reason == 2) {
    btSerialWrite('2');
  } else if (reason == 3) {
    btSerialWrite('3');
  } else if (reason == 4) {
    btSerialWrite('4');
  } else if (reason == 5) {
    btSerialWrite('5');
  } else if (reason == 6) {
    btSerialWrite('6');
  } else if (reason == 7) {
    btSerialWrite('7');
  } else if (reason == 8) {
    btSerialWrite('8');
  }

  btSerialWrite('\n');
}

void btSerialWrite(char data) {
  btLastCommandTime = millis();
  BTSerial.write(data);
}

void processBluetooth() {
  if (BTSerial.available()) {
    btLastCommand = BTSerial.read();

    if (btLastCommand == BT_COMMAND_SET_MODE_MANUAL) {
      setMode(MODE_MANUAL, SWITCH_MODE_REASON_BLUETOOTH_COMMAND);
    } else if (btLastCommand == BT_COMMAND_SET_MODE_FOLLOW_LINE) {
      setMode(MODE_FOLLOW_LINE, SWITCH_MODE_REASON_BLUETOOTH_COMMAND);
    } else if (btLastCommand == BT_COMMAND_SET_MODE_TAKE_PACKAGE) {
      setMode(MODE_TAKE_PACKAGE, SWITCH_MODE_REASON_BLUETOOTH_COMMAND);
    }
    
    buzz(1);
  } else if (mode == MODE_MANUAL && (btLastCommandTime + 10000) < millis()) {
    btSerialWrite('0');
    btSerialWrite('\n');
  }

  if (mode == 1) {
    if (btLastCommand == BT_COMMAND_ARM_LEFT_PRESSED) {
      armTurnLeft();
    } else if (btLastCommand == BT_COMMAND_ARM_RIGHT_PRESSED) {
      armTurnRight();
    } else if (btLastCommand == BT_COMMAND_ARM_FORWARD_PRESSED) {
      armForward();
    } else if (btLastCommand == BT_COMMAND_ARM_BACK_PRESSED) {
      armBack();
    } else if (btLastCommand == BT_COMMAND_ARM_UP_PRESSED) {
      armUp();
    } else if (btLastCommand == BT_COMMAND_ARM_DOWN_PRESSED) {
      armDown();
    } else if (btLastCommand == BT_COMMAND_ARM_OPEN_CLAW_PRESSED) {
      openClaw();
    } else if (btLastCommand == BT_COMMAND_ARM_CLOSE_CLAW_PRESSED) {
      closeClaw();
    } else if (btLastCommand == BT_COMMAND_ARM_CLOSE_CLAW_PRESSED) {
      closeClaw();
    } else if (btLastCommand == BT_COMMAND_WHEELS_FORWARD_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
  
      rightForwardStart();
      leftForwardStart();
    } else if (btLastCommand == BT_COMMAND_WHEELS_LEFT_FORWARD_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
      
      rightForwardStart();
    } else if (btLastCommand == BT_COMMAND_WHEELS_RIGHT_FORWARD_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
      
      leftForwardStart();
    } else if (btLastCommand == BT_COMMAND_WHEELS_BACK_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
      
      rightBackStart();
      leftBackStart();
    } else if (btLastCommand == BT_COMMAND_WHEELS_LEFT_BACK_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
      
      rightBackStart();
    } else if (btLastCommand == BT_COMMAND_WHEELS_RIGHT_BACK_PRESSED) {
      analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
      analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);
      
      leftBackStart();
    } 
  }
}
