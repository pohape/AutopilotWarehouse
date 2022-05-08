#include <IRremote.h>
//#include "SR04.h"
#include <Servo.h>

// PINs >>>

// ultrasonic
const int ULTRASONIC_TRIG_PIN = A1;
const int ULTRASONIC_ECHO_PIN = A0;

// tracing
const int PIN_TRACING_RIGHT = A2;
const int PIN_TRACING_CENTER = 10;
const int PIN_TRACING_LEFT = A5;

// infrared
const int PIN_INFRARED = 11;

// wheels
const int PIN_WHEELS_ENA = 5;
const int PIN_WHEELS_ENB = 6;
const int PIN_WHEELS_IN1 = A4;
const int PIN_WHEELS_IN2 = A3;
const int PIN_WHEELS_IN3 = 7;
const int PIN_WHEELS_IN4 = 8;

// arm
const int PIN_ARM_MAIN = 4;
const int PIN_ARM_LEFT = 9;
const int PIN_ARM_RIGHT = 2;
const int PIN_ARM_CLAW = 3;

// <<< PINs

// ultrasonic block >>>
//SR04 ultrasonic = SR04(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIG_PIN);
// <<< ultrasonic block

// infrared block >>>
IRrecv infrared(PIN_INFRARED);
decode_results infraredResults;

String codeHold = "4294967295";
String one = "16753245";

String two = "16736925";
String three = "16769565";
String four = "16720605";
String five = "16712445";
String six = "16761405";
String seven = "16769055";
String eight = "16754775";
String nine = "16748655";
String zero = "16750695";

String star = "16738455";
String grid = "16756815";
String ok = "16726215";
String up = "16718055";
String down = "16730805";
String left = "16716015";
String right = "16734885";

String remotePushed;
String remoteLastPushed;
// <<< infrared block

// arm block >>>
Servo armServoMain;
Servo armServoRight;
Servo armServoLeft;
Servo armServoClaw;

const int armPositionMainCenter = 72; // 80 - center, correction is -8
const int armPositionMainMin = 27; // 35, with correction is 27
const int armPositionMainMax = 117; // 125, with correction is 117
int armPositionMain = 0; // unknown at start

int armPositionRight = 60; // 20 - поднята, 80 - опущена
int armPositionLeft = 140; // 0 - вытянута, 140 - втянута
int armPositionClaw = 0; // 0 - закрыто, 100 - открыто
// <<< arm block

// tracing block >>>
// 0 - empty
// 1 - forward left
// 2 - forward
// 3 - forward right
// 7 - back left
// 8 - back
// 9 - back right
int lastFollowLineMoves[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// <<< tracing block

// wheels block >>>
int wheelsSpeed = 108; // from 0 to 255;
int oneMoveBothMs = 130;
int oneMoveSingleMs = 150;

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

// 1 = manual
// 2 = follow the line
int mode = 1;

void setup() {
  Serial.begin(9600);  // speed for the console
  infrared.enableIRIn(); // Start the infrared receiver

  pinMode(PIN_TRACING_RIGHT, INPUT);
  pinMode(PIN_TRACING_CENTER, INPUT);
  pinMode(PIN_TRACING_LEFT, INPUT);

  pinMode(PIN_WHEELS_ENA, OUTPUT);
  pinMode(PIN_WHEELS_ENB, OUTPUT);
  analogWrite(PIN_WHEELS_ENA, wheelsSpeed);
  analogWrite(PIN_WHEELS_ENB, wheelsSpeed);

  pinMode(PIN_WHEELS_IN1, OUTPUT);
  pinMode(PIN_WHEELS_IN2, OUTPUT);
  pinMode(PIN_WHEELS_IN3, OUTPUT);
  pinMode(PIN_WHEELS_IN4, OUTPUT);

  armServoMain.attach(PIN_ARM_MAIN);
//  armServoRight.attach(PIN_ARM_RIGHT);
//  armServoLeft.attach(PIN_ARM_LEFT);
//  armServoClaw.attach(PIN_ARM_CLAW);

  armServoMain.write(armPositionMainCenter);
  armPositionMain = armPositionMainCenter;
  delay(100);
  
  armServoLeft.write(armPositionLeft);
  delay(100);
  armServoRight.write(armPositionRight);
  delay(100);
  armServoClaw.write(armPositionClaw);
}

void loop() {
  //checkDistance();
  processIrButtons();
  processFollowLine();
  manageStateOfWheels();
}
