const int BT_COMMAND_ARM_FORWARD_PRESSED = 1;
const int BT_COMMAND_ARM_FORWARD_RELEASED = 2;

const int BT_COMMAND_ARM_BACK_PRESSED = 3;
const int BT_COMMAND_ARM_BACK_RELEASED = 4;

const int BT_COMMAND_ARM_LEFT_PRESSED = 5;
const int BT_COMMAND_ARM_LEFT_RELEASED = 6;

const int BT_COMMAND_ARM_RIGHT_PRESSED = 7;
const int BT_COMMAND_ARM_RIGHT_RELEASED = 8;

const int BT_COMMAND_ARM_UP_PRESSED = 9;
const int BT_COMMAND_ARM_UP_RELEASED = 10;

const int BT_COMMAND_ARM_DOWN_PRESSED = 11;
const int BT_COMMAND_ARM_DOWN_RELEASED = 12;

const int BT_COMMAND_ARM_OPEN_CLAW_PRESSED = 13;
const int BT_COMMAND_ARM_OPEN_CLAW_RELEASED = 14;

const int BT_COMMAND_ARM_CLOSE_CLAW_PRESSED = 15;
const int BT_COMMAND_ARM_CLOSE_CLAW_RELEASED = 16;

int btLastCommand = 0;

void processBluetooth() {
  if (mode == 1) {
    if (BTSerial.available()) {
      btLastCommand = BTSerial.read();
      buzz(1);
    }

    //Serial.println(btLastCommand);
    
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
    }
  }
} 
