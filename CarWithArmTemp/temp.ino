#include <IRremote.h>
#include <Servo.h>
#include "SR04.h"

int BUZZER_PIN = 1;
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

const int tracingPinRight = A2;
const int tracingPinCenter = 10;
const int tracingPinLeft = A5;

// posMain: 80 - center, 125 - left, 35 - right
// posLeft: 0 - вытянута, 140 - втянута
// posRight: 20 - поднята, 80 - опущена
// posClaw: 0 - закрыто, 100 - открыто
int posMain = 80, posRight = 60, posLeft = 140, posClaw = 0;
int posMainCorrection = -8;

const int ledPin = 13; // Built in LED in Arduino board
String msg, cmd;

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

String codeButtonPushed;
String lastButtonPushedSymbol;

int wheelsSpeed = 108; // from 0 to 255;
int enA = 5;
int enB = 6;
int IN1 = A4;
int IN2 = A3;
int IN3 = 7;
int IN4 = 8;

int oneMoveBothMs = 130;
int oneMoveSingleMs = 150;

// 0 - empty
// 1 - forward left
// 2 - forward
// 3 - forward right
// 7 - back left
// 8 - back
// 9 - back right
int lastFollowLineMoves[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Keeping the current state of the wheels >>>
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
// <<< Keeping the current state of the wheels

// 1 = manual
// 2 = follow the line
// 3 = bypass an obstacle
int mode = 1;

// ultrasonic block
const int TRIG_PIN = A1;
const int ECHO_PIN = A0;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

Servo servoMain; // create servo object to control a servo

void setup() {
  Serial.begin(9600);  // speed for the console
  irrecv.enableIRIn(); // Start the infrared receiver

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(tracingPinRight, INPUT);
  pinMode(tracingPinCenter, INPUT);
  pinMode(tracingPinLeft, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  analogWrite(enA, wheelsSpeed);
  analogWrite(enB, wheelsSpeed);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servoMain.attach(4); // attaches the servo on pin 9 to the servo object
  servoMain.write(posMain + posMainCorrection);
  delay(100);
}

void loop() {
  //Serial.println((millis() / 100) % 10);
//  if (millis() % 100 == 0) {
//      checkDistance();
//  }
  
  processIrButtons();
  processFollowLine();
  manageStateOfWheels();
}

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

void checkDistance() {
  long distance = sr04.Distance();
  Serial.print(distance);
  Serial.println("cm");
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

void processMode2() {
    // найти черную линию
    // проехать вперед долю секунды
    digitalWrite(ledPin, HIGH);

    int center = digitalRead(tracingPinCenter);

    if (center == HIGH) {
      long distance = sr04.Distance();

      if (distance < 30) {
        bothStop();
        mode = 3;
      } else {      
        addMoveToLastMovesArray(2);
        rightForwardStart();
        leftForwardStart();
      }
    } else {
      bothStop();

      int right = digitalRead(tracingPinRight);
      int left = digitalRead(tracingPinLeft);

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
        digitalWrite(ledPin, LOW);

        return;
      }
    }

    followLineCheckAndStop();
}

void processMode3() {
  long distance = sr04.Distance();

  if (distance < 30) {
    bothStop();
    leftForwardStart();
    delay(500);
  }
  else if (posMain > 70 && posMain < 90) {
    posMain = 125;
    servoMain.write(125 + posMainCorrection);
    delay(100);
    leftForwardStart();
    rightForwardStart();
    delay(1000);
    bothStop();
  }
  else {
    int center = digitalRead(tracingPinCenter);
    
    if (center == HIGH) {
       mode = 2;
       posMain = 80;
       servoMain.write(80 + posMainCorrection);

       delay(100);
    } else {
      rightForwardStart();

      if ((millis() / 100) % 10 <= 4) {
        leftForwardStart();
      }
    }
  }
  
//  else if (posMain > 70 && posMain < 90) {
//    posMain = 60;
//    servoMain.write(60 + posMainCorrection);
//    delay(100);
//  } else if (posMain > 90 && posMain < 110) {
//    posMain = 100;
//    servoMain.write(100 + posMainCorrection);
//    delay(100);
//  } else if (posMain > 110 && posMain < 130) {
//    posMain = 125;
//    servoMain.write(125 + posMainCorrection);
//    delay(100);
//  } else {
//    bothStop();
//    int center = digitalRead(tracingPinCenter);
//
//    if (center == HIGH) {
//       mode = 2;
//       posMain = 80;
//       servoMain.write(80 + posMainCorrection);
//
//       delay(100);
//    } else {
//      rightForwardStart();
//    }
//  }
}

void processFollowLine() {
  if (mode == 2) {
    processMode2();
  } else if (mode == 3) {
    processMode3();
  }
}

void beep(int times) {
//  for (int i = 0; i < times; i++) {
//    Serial.println("buzzer on");
//    tone(BUZZER_PIN, 10000);
//    delay(250);
//    Serial.println("buzzer off");
//    noTone(BUZZER_PIN);
//    delay(250);
//  }
}

void processIrButtons() {
  if (irrecv.decode( & results)) {
    codeButtonPushed = String(results.value);

    Serial.println(codeButtonPushed);

    if (mode == 1 && codeButtonPushed == one) {
      lastButtonPushedSymbol = one;

      rightForwardStart();
    } else if (mode == 1 && codeButtonPushed == two) {
      lastButtonPushedSymbol = two;

      rightForwardStart();
      leftForwardStart();
    } else if (mode == 1 && codeButtonPushed == three) {
      lastButtonPushedSymbol = three;

      leftForwardStart();
    } else if (codeButtonPushed == five) {
      lastButtonPushedSymbol = five;

      if (mode == 1) {
        mode = 2;
      } else {
        mode = 1;
      }

      Serial.println("mode " + String(mode));
    } else if (mode == 1 && codeButtonPushed == seven) {
      lastButtonPushedSymbol = seven;

      rightBackStart();
    } else if (mode == 1 && codeButtonPushed == eight) {
      lastButtonPushedSymbol = eight;

      rightBackStart();
      leftBackStart();
    } else if (mode == 1 && codeButtonPushed == nine) {
      lastButtonPushedSymbol = nine;

      leftBackStart();
    } 

    if (codeButtonPushed == codeHold) {
      Serial.println(String(millis()) + ": hold " + lastButtonPushedSymbol);

      if (lastButtonPushedSymbol == two) {
        rightForwardStart();
        leftForwardStart();
      } else if (lastButtonPushedSymbol == eight) {
        rightBackStart();
        leftBackStart();
      } else if (lastButtonPushedSymbol == one) {
        rightForwardStart();
      } else if (lastButtonPushedSymbol == three) {
        leftForwardStart();
      } else if (lastButtonPushedSymbol == seven) {
        rightBackStart();
      } else if (lastButtonPushedSymbol == nine) {
        leftBackStart();
      }
    } else {
      Serial.println(String(millis()) + ": pushed " + lastButtonPushedSymbol);
    }

    irrecv.resume(); // Receive the next value
  }
}

void manageStateOfWheels() {
  unsigned long currentMillis = millis();

  // if the wheel is moving more than $oneMoveMs ms
  if (leftCurrentlyMovingForward && rightCurrentlyMovingForward && (currentMillis - leftForwardStarted) > oneMoveBothMs && (currentMillis - rightForwardStarted) > oneMoveBothMs) {
    bothStop();
  } else if (leftCurrentlyMovingBack && rightCurrentlyMovingBack && (currentMillis - leftBackStarted) > oneMoveBothMs && (currentMillis - rightBackStarted) > oneMoveBothMs) {
    bothStop();
  } else if (leftCurrentlyMovingForward && (currentMillis - leftForwardStarted) > oneMoveSingleMs) {
    Serial.println(String(currentMillis) + ": leftCurrentlyMovingForward && leftForwardStarted " + String(leftForwardStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - leftForwardStarted));

    leftStop();
  } else if (rightCurrentlyMovingForward && (currentMillis - rightForwardStarted) > oneMoveSingleMs) {
    Serial.println(String(currentMillis) + ": rightCurrentlyMovingForward && rightForwardStarted " + String(rightForwardStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - rightForwardStarted));

    rightStop();
  } else if (leftCurrentlyMovingBack && (currentMillis - leftBackStarted) > oneMoveSingleMs) {
    Serial.println(String(currentMillis) + ": leftCurrentlyMovingBack && leftBackStarted " + String(leftBackStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - leftBackStarted));

    leftStop();
  } else if (rightCurrentlyMovingBack && (currentMillis - rightBackStarted) > oneMoveSingleMs) {
    Serial.println(String(currentMillis) + ": rightCurrentlyMovingBack && rightBackStarted " + String(rightBackStarted) + "; oneMoveMs " + String(oneMoveSingleMs) + "; working for " + String(currentMillis - rightBackStarted));

    rightStop();
  }
}

// Wheels left/right start/stop functions >>>
void rightForwardStart() {
  unsigned long currentMillis = millis();

  Serial.println(String(currentMillis) + ": Right forward start");
  rightForwardStarted = currentMillis;
  rightForwardStopped = 0;
  rightCurrentlyMovingForward = true;
  rightCurrentlyMovingBack = false;

  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);
}

void rightBackStart() {
  unsigned long currentMillis = millis();

  Serial.println(String(currentMillis) + ": Right back start");
  rightBackStarted = currentMillis;
  rightBackStopped = 0;
  rightCurrentlyMovingForward = false;
  rightCurrentlyMovingBack = true;

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftForwardStart() {
  unsigned long currentMillis = millis();

  Serial.println(String(currentMillis) + ": Left forward start");
  leftForwardStarted = currentMillis;
  leftForwardStopped = 0;
  leftCurrentlyMovingForward = true;

  digitalWrite(IN2, LOW);
  digitalWrite(IN1, HIGH);
}

void leftBackStart() {
  unsigned long currentMillis = millis();

  Serial.println(String(currentMillis) + ": Left back start");
  leftBackStarted = currentMillis;
  leftBackStopped = 0;
  leftCurrentlyMovingForward = false;
  leftCurrentlyMovingBack = true;

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void leftStop() {
  leftCurrentlyMovingForward = false;
  leftCurrentlyMovingBack = false;

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.println(String(millis()) + ": Left stopped");
}

void rightStop() {
  rightCurrentlyMovingForward = false;
  rightCurrentlyMovingBack = false;

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.println(String(millis()) + ": Right stopped");
}

void bothStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  rightCurrentlyMovingForward = false;
  rightCurrentlyMovingBack = false;
  leftCurrentlyMovingForward = false;
  leftCurrentlyMovingBack = false;

  Serial.println(String(millis()) + ": Both stopped");
}

// <<< Wheels left/right start/stop functions

// Arm functions >>>

void armTurnRight() {
  Serial.println("Turn right\n");

  int minPos = 35 + posMainCorrection;
  int targetPos = posMain + posMainCorrection - 45;

  if (targetPos < minPos) {
    targetPos = minPos;
  }

  for (posMain; posMain > targetPos; posMain--) {
    servoMain.write(posMain);
    delay(5); // delay 5ms（used to adjust the servo speed）
  }

  delay(100);
}

void armTurnLeft() {
  Serial.println("Turn left\n");

  int maxPos = 125 + posMainCorrection;
  int targetPos = posMain + posMainCorrection + 45;

  if (targetPos > maxPos) {
    targetPos = maxPos;
  }

  for (posMain; posMain < targetPos; posMain++) {
    servoMain.write(posMain);
    delay(5);
  }

  delay(100);
}

// <<< Arm functions
