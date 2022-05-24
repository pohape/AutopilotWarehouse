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

const int BT_COMMAND_WHEELS_FORWARD_PRESSED = 17;
const int BT_COMMAND_WHEELS_FORWARD_RELEASED = 18;

const int BT_COMMAND_WHEELS_LEFT_FORWARD_PRESSED = 19;
const int BT_COMMAND_WHEELS_LEFT_FORWARD_RELEASED = 20;

const int BT_COMMAND_WHEELS_RIGHT_FORWARD_PRESSED = 21;
const int BT_COMMAND_WHEELS_RIGHT_FORWARD_RELEASED = 22;

const int BT_COMMAND_WHEELS_BACK_PRESSED = 23;
const int BT_COMMAND_WHEELS_BACK_RELEASED = 24;

const int BT_COMMAND_WHEELS_LEFT_BACK_PRESSED = 25;
const int BT_COMMAND_WHEELS_LEFT_BACK_RELEASED = 26;

const int BT_COMMAND_WHEELS_RIGHT_BACK_PRESSED = 27;
const int BT_COMMAND_WHEELS_RIGHT_BACK_RELEASED = 28;

const int BT_COMMAND_SWITCH_MODE = 29;

int btLastCommand = 0;

void processBluetooth() {
  if (BTSerial.available()) {
    btLastCommand = BTSerial.read();

    if (btLastCommand == BT_COMMAND_SWITCH_MODE) {
      if (mode == 1) {
        setMode(2, "Bluetooth command");
      } else {
        setMode(1, "Bluetooth command");
      }
    }
    
    buzz(1);
  }

  //Serial.println(btLastCommand);

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
