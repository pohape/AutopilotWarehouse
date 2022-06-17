#include <IRremote.hpp>
#include "SR04.h"
#include <Servo.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

// PINs >>>

// ultrasonic
const int ULTRASONIC_TRIG_PIN = A1;
const int ULTRASONIC_ECHO_PIN = A0;

// tracing
const int PIN_TRACING_RIGHT = A2; // S = black, G - brown, V - white
const int PIN_TRACING_CENTER = A3; // S = black, G - brown, V - white
const int PIN_TRACING_LEFT = A4; // S = black, G - brown, V - white

// infrared
const int PIN_INFRARED_CLAW_DISTANCE = A5;

// wheels
const int PIN_WHEELS_ENA = 6; // green, first (left)
const int PIN_WHEELS_ENB = 7; // red, last (right)
const int PIN_WHEELS_IN1 = 8; // yellow, second from left
const int PIN_WHEELS_IN2 = 9; // brown, 3rd from left
const int PIN_WHEELS_IN3 = 10; // blue, 4th from left
const int PIN_WHEELS_IN4 = 11; // black, 5th from left

// arm
const int PIN_ARM_MAIN = 3;
const int PIN_ARM_LEFT = 4;
const int PIN_ARM_RIGHT = 5;
const int PIN_ARM_CLAW = 52;

// bluetooth
const int PIN_BLUETOOTH_RX = 53;
const int PIN_BLUETOOTH_TX = 51;

const int PIN_BUZZER = 2;

// <<< PINs

// bluetooth block >>>
String btResponce = ""; // Stores response of the HC-06 Bluetooth device
SoftwareSerial BTSerial(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX); // RX, TX

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

const int BT_COMMAND_TURN_AROUND = 29;

const int BT_COMMAND_SET_MODE_MANUAL = 101;
const int BT_COMMAND_SET_MODE_FOLLOW_LINE = 102;
const int BT_COMMAND_SET_MODE_TAKE_PACKAGE = 104;
const int BT_COMMAND_SET_MODE_LEAVE_PACKAGE = 105;

int btLastCommand = 0;
unsigned long btLastCommandTime = 0;
// <<< bluetooth block

// QR-code scanner block >>>
#define QRSerial Serial3
// <<< QR-code scanner block

// ultrasonic block >>>
SR04 ultrasonic = SR04(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIG_PIN);
const int DISTANCE_WARNING = 25;
const int MIN_DISTANCE_TO_PACKAGE = 10;
const int MAX_DISTANCE_TO_PACKAGE = 18;
int distance = 0;
int obstacles = 0;
// <<< ultrasonic block

// infrared distance block >>>
const int CLAW_DISTANCE_HOLD_MIN = 839;
const int CLAW_DISTANCE_HOLD_MAX = 940;
const int CLAW_DISTANCE_TAKE = 890;
const int CLAW_DISTANCE_HOVER = 960;
int clawDistance = 0;
// <<< infrared distance block

// arm block >>>
Servo armServoMain;
Servo armServoRight;
Servo armServoLeft;
Servo armServoClaw;

const int ARM_SERVOS_STEP = 1;

const int ARM_POSITION_MAIN_DEFAULT = 72; // 80 - center, correction is -8
const int ARM_POSITION_MAIN_MIN = 2; // 10, with correction is 2
const int ARM_POSITION_MAIN_MAX = 137; // 145, with correction is 137

const int ARM_POSITION_RIGHT_DEFAULT = 0; // 20 - поднята, 110 - опущена
const int ARM_POSITION_RIGHT_MIN = 0;
const int ARM_POSITION_RIGHT_MAX = 110;

const int ARM_POSITION_LEFT_DEFAULT = 110; // 0 - вытянута, 120 - втянута
const int ARM_POSITION_LEFT_MIN = 0;
const int ARM_POSITION_LEFT_MAX = 120;

const int CLAW_POSITION_DEFAULT = 0; // 0 - закрыто, 50 - открыто
const int CLAW_POSITION_MIN = 0;
const int CLAW_POSITION_RELEASE = 20;
const int CLAW_POSITION_MAX = 80;

const int PACKAGE_NOT_FOUND_BY_ULTRASONIC = 1;
const int PACKAGE_NOT_FOUND_BY_INFRARED_HOVER = 2;
const int PACKAGE_NOT_FOUND_BY_INFRARED_TAKE = 3;
const int PACKAGE_LOST = 4;

struct ServoPositions {
   int armMain;
   int armLeft;
   int armRight;
   int armClaw;
};

ServoPositions servoPositions = {ARM_POSITION_MAIN_DEFAULT, ARM_POSITION_LEFT_DEFAULT, ARM_POSITION_RIGHT_DEFAULT};

const int ARM_TAKE_PACKAGE_POSITIONS_COUNT = 8;
const int ARM_TAKE_PACKAGE_POSITIONS[ARM_TAKE_PACKAGE_POSITIONS_COUNT][2] = { {15, 110}, {15, 100}, {10, 90}, {0, 90}, {10, 83}, {0, 80}, {0, 75}, {0, 68} };

const int ARM_HOVER_POSITIONS_COUNT = 6;
const int ARM_HOVER_POSITIONS[ARM_HOVER_POSITIONS_COUNT][2] = {{30, 90}, {28, 80}, {19, 58}, {7, 54}, {19, 58}, {28, 80} };
const int ARM_LEAVE_PACKAGE_TOP_POSITIONS[6][2] = { {87, 13}, {70, 18}, {60, 20}, {50, 22}, {40, 20}, {30, 20} };
//const int ARM_LEAVE_PACKAGE_TOP_POSITIONS[6][2] = { {97, 0}, {87, 0}, {77, 0}, {67, 0}, {47, 0}, {42, 16} };
//const int ARM_LEAVE_PACKAGE_BOTTOM_POSITIONS[4][2] = { {40, 74}, {32, 49}, {15, 39}, {0, 33} };

bool holdingPackage = false;
// <<< arm block

// tracing block >>>
// 0 - empty
// 1 - forward left
// 2 - forward
// 3 - forward right
// 7 - back left
// 8 - back
// 9 - back right
unsigned long followLineStarted = 0;
int lastFollowLineMoves[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int allHighInRowCount = 0;
const int ALL_HIGH_IN_ROW_COUNT_TO_STOP = 12;
const int MAX_BACK_IN_ROW_TO_STOP = 1000;
// <<< tracing block

// wheels block >>>
const int WHEELS_SPEED_DEFAULT = 95; // from 0 to 255;
const int WHEELS_SPEED_TO_GO_AROUND_OBSTACLE_INSIDE = 45;
const int WHEELS_SPEED_TO_GO_AROUND_OBSTACLE_OUTSIDE = 110;
const int WHEELS_SPEED_TO_GO_BACK = 60;
const int WHEELS_SPEED_TO_TURN = 120;
const int WHEELS_SPEED_TURN_AROUND = 110;

const int ONE_WHEEL_TURN_DELAY = 50;
const int SEARCH_LINE_BACK_DELAY = 40;

const int ONE_MOVE_BOTH_MS = 130;
const int ONE_MOVE_SINGLE_MS = 150;

// keeping the current state of the wheels >>
unsigned long leftForwardStarted = 0;
unsigned long leftForwardStopped = 0;
bool leftCurrentlyMovingForward = false;

unsigned long leftBackStarted = 0;
unsigned long leftBackStopped = 0;
bool leftCurrentlyMovingBack = false;

unsigned long rightForwardStarted = 0;
unsigned long rightForwardStopped = 0;
bool rightCurrentlyMovingForward = false;

unsigned long rightBackStarted = 0;
unsigned long rightBackStopped = 0;
bool rightCurrentlyMovingBack = false;

// << keeping the current state of the wheels
// <<< wheels block

const int MODE_MANUAL = 1;
const int MODE_FOLLOW_LINE = 2;
const int MODE_AROUND_OBSTACLE = 3;
const int MODE_TAKE_PACKAGE = 4;
const int MODE_LEAVE_PACKAGE = 5;

const int SWITCH_MODE_REASON_LINE_LOST = 1;
const int SWITCH_MODE_REASON_LINE_ENDED = 2;
const int SWITCH_MODE_REASON_FOUND_LINE = 3;
const int SWITCH_MODE_REASON_FOUND_OBSTACLE = 4;
const int SWITCH_MODE_REASON_PACKAGE_NOT_FOUND = 5;
const int SWITCH_MODE_REASON_PACKAGE_LOST = 6;
const int SWITCH_MODE_REASON_HOLD_PACKAGE = 7;
const int SWITCH_MODE_REASON_BLUETOOTH_COMMAND = 8;
const int SWITCH_MODE_REASON_RELEASED_PACKAGE = 9;

int mode = 1;

void setup() {
  delay(500);
  Serial.begin(9600);  // speed for the console
  pinMode(PIN_INFRARED_CLAW_DISTANCE, INPUT);

  pinMode(PIN_TRACING_RIGHT, INPUT);
  pinMode(PIN_TRACING_CENTER, INPUT);
  pinMode(PIN_TRACING_LEFT, INPUT);

  pinMode(PIN_WHEELS_ENA, OUTPUT);
  pinMode(PIN_WHEELS_ENB, OUTPUT);

  pinMode(PIN_WHEELS_IN1, OUTPUT);
  pinMode(PIN_WHEELS_IN2, OUTPUT);
  pinMode(PIN_WHEELS_IN3, OUTPUT);
  pinMode(PIN_WHEELS_IN4, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  initializeArm();
  
  BTSerial.begin(9600);
  QRSerial.begin(9600);
  delay(500);
}

void loop() {
  processBluetooth();
  processQrCodeScanner();
  manageStateOfWheels();

  if (mode == 2) {
    processMode2();
  } else if (mode == 3) {
    processMode3();
  }
}

void buzz(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_BUZZER,HIGH);
    delay(1);
    digitalWrite(PIN_BUZZER,LOW);
  }
}

void setMode(int newMode, int reason) {
  Serial.println("setMode: " + String(newMode));
  Serial.println("reason: " + String(reason));

  if (reason == SWITCH_MODE_REASON_LINE_LOST) {
    Serial.println("LINE_LOST: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_LINE_ENDED) {
    Serial.println("LINE_ENDED: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_FOUND_LINE) {
    Serial.println("FOUND_LINE: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_FOUND_OBSTACLE) {
    Serial.println("FOUND_OBSTACLE: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_PACKAGE_NOT_FOUND) {
    Serial.println("PACKAGE_NOT_FOUND: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_PACKAGE_LOST) {
    Serial.println("PACKAGE_LOST: set mode " + String(newMode));
  } else if (reason == SWITCH_MODE_REASON_HOLD_PACKAGE) {
    Serial.println("HOLD_PACKAGE: set mode " + String(newMode));
  }

  btSetMode(newMode, reason);
  buzz(50);
  mode = newMode;

  if (newMode == MODE_FOLLOW_LINE) {
    followLineStarted = millis();
    armTurnCenter();
    armToDefaultPosition();
  } else if (newMode == MODE_TAKE_PACKAGE) {
    findAndTakePackage();
  } else if (newMode == MODE_LEAVE_PACKAGE) {
    findShelfAndLeavePackage();
  } else if (newMode == MODE_MANUAL) {
    followLineStarted = 0;
    obstacles = 0;
  }
}
