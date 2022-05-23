#include <IRremote.hpp>
#include "SR04.h"
#include <Servo.h>
#include <EEPROM.h>

// PINs >>>

// ultrasonic
const int ULTRASONIC_TRIG_PIN = A1;
const int ULTRASONIC_ECHO_PIN = A0;

// tracing
const int PIN_TRACING_RIGHT = A2; // S = black, G - brown, V - white
const int PIN_TRACING_CENTER = A3; // S = black, G - brown, V - white
const int PIN_TRACING_LEFT = A4; // S = black, G - brown, V - white

// infrared
const int PIN_INFRARED = A5;

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
const int PIN_ARM_CLAW = 53;

const int PIN_BUZZER = 2;

// <<< PINs

// ultrasonic block >>>
SR04 ultrasonic = SR04(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIG_PIN);
const int DISTANCE_WARNING = 25;
int distance = 0;
// <<< ultrasonic block

// infrared block >>>
IRrecv infrared(PIN_INFRARED);
decode_results infraredResults;

uint32_t one = 0xBA45FF00;
uint32_t two = 0xB946FF00;
uint32_t three = 0xB847FF00;
uint32_t four = 0xBB44FF00;
uint32_t five = 0xBF40FF00;
uint32_t six = 0xBC43FF00;
uint32_t seven = 0xF807FF00;
uint32_t eight = 0xEA15FF00;
uint32_t nine = 0xF609FF00;
uint32_t zero = 0xE619FF00;

uint32_t star = 0xE916FF00;
uint32_t grid = 0xF20DFF00;
uint32_t ok = 0xE31CFF00;
uint32_t up = 0xE718FF00;
uint32_t down = 0xAD52FF00;
uint32_t left = 0xF708FF00;
uint32_t right = 0xA55AFF00;

uint32_t remoteLastPushed;
// <<< infrared block

// arm block >>>
Servo armServoMain;
Servo armServoRight;
Servo armServoLeft;
Servo armServoClaw;

struct ServoPositions {
   int armMain;
   int armLeft;
   int armRight;
   int armClaw;
};

const int ARM_SERVOS_STEP = 5;

const int ARM_POSITION_MAIN_DEFAULT = 72; // 80 - center, correction is -8
const int ARM_POSITION_MAIN_MIN = 2; // 10, with correction is 2
const int ARM_POSITION_MAIN_MAX = 137; // 145, with correction is 137

const int ARM_POSITION_RIGHT_DEFAULT = 40; // 20 - поднята, 80 - опущена
const int ARM_POSITION_RIGHT_MIN = 0;
const int ARM_POSITION_RIGHT_MAX = 80;

const int ARM_POSITION_LEFT_DEFAULT = 140; // 0 - вытянута, 140 - втянута
const int ARM_POSITION_LEFT_MIN = 0;
const int ARM_POSITION_LEFT_MAX = 170;

int armPositionClawDefault = 0; // 0 - закрыто, 50 - открыто
int armPositionClawMin = 0;
int armPositionClawMax = 50;

ServoPositions servoPositions = {ARM_POSITION_MAIN_DEFAULT, ARM_POSITION_LEFT_DEFAULT, ARM_POSITION_RIGHT_DEFAULT};
// <<< arm block

// tracing block >>>
// 0 - empty
// 1 - forward left
// 2 - forward
// 3 - forward right
// 7 - back left
// 8 - back
// 9 - back right
int lastFollowLineMoves[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int MAX_BACK_IN_ROW_TO_STOP = 15;
// <<< tracing block

// wheels block >>>
const int WHEELS_SPEED_DEFAULT = 90; // from 0 to 255;
const int WHEELS_SPEED_TO_GO_AROUND_OBSTACLE_INSIDE = 45;
const int WHEELS_SPEED_TO_GO_AROUND_OBSTACLE_OUTSIDE = 110;
const int WHEELS_SPEED_TO_GO_BACK = 60;
const int WHEELS_SPEED_TO_TURN = 120;

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

int backInRowCount = 0;
// << keeping the current state of the wheels
// <<< wheels block

// 1 = manual
// 2 = follow the line
// 3 = drive around an obstacle
int mode = 1;

void setup() {
  delay(500);
  Serial.begin(9600);  // speed for the console
  infrared.enableIRIn(); // Start the infrared receiver

  pinMode(PIN_TRACING_RIGHT, INPUT);
  pinMode(PIN_TRACING_CENTER, INPUT);
  pinMode(PIN_TRACING_LEFT, INPUT);

  pinMode(PIN_WHEELS_ENA, OUTPUT);
  pinMode(PIN_WHEELS_ENB, OUTPUT);
  analogWrite(PIN_WHEELS_ENA, WHEELS_SPEED_DEFAULT);
  analogWrite(PIN_WHEELS_ENB, WHEELS_SPEED_DEFAULT);

  pinMode(PIN_WHEELS_IN1, OUTPUT);
  pinMode(PIN_WHEELS_IN2, OUTPUT);
  pinMode(PIN_WHEELS_IN3, OUTPUT);
  pinMode(PIN_WHEELS_IN4, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);

  initializeArm(); 
}

void loop() {
//  int right = digitalRead(PIN_TRACING_RIGHT);
//  int center = digitalRead(PIN_TRACING_CENTER);
//  int left = digitalRead(PIN_TRACING_LEFT);
//
//  Serial.println("Left: " + String(left));
//  Serial.println("Center: " + String(center));
//  Serial.println("Right: " + String(right));
//  Serial.println("");
//  delay(500);
  
  processIrButtons();
  processFollowLine();
  manageStateOfWheels();
}

void buzz(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_BUZZER,HIGH);
    delay(1);
    digitalWrite(PIN_BUZZER,LOW);
  }
}

void setMode(int m, String caller) {
  Serial.println(caller + ": set mode " + String(m));

//  if ((mode == 1 && m == 2) || (mode == 2 && m == 1)) {
//    bothStop();
//    delay(100);
//  }
  
  mode = m;
  buzz(50);

  if (m == 2) {
    armTurnCenter();
  }
}
