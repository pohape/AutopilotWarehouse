#include <Servo.h>
#include "SR04.h"
#include <IRremote.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// ultrasonic block
const int TRIG_PIN = 8;
const int ECHO_PIN = 7;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);


// servo block
const int SERVO_PIN = 9;
const int RIGHT_POS = 0;
const int CENTER_POS = 90;
const int LEFT_POS = 180;
int pos = 90;    // variable to store the servo position


// infra red block
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
String codeHold = "4294967295";
String left = "16716015";
String right = "16734885";
String codeButtonPushed;
String lastButtonPushedSymbol;


void setup() {
  Serial.begin(9600); // speed for console
  irrecv.enableIRIn(); // Start the receiver

  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
  delay(500);
}

void moveFromTo(int from, int to){
  if (from < 0) {
    from = 0;
  }

  if (to > 180) {
    to = 180;
  }

  pos = from;

  while (pos != to) { // пока текущая позиция не ровна целевой позиции, то продолжаем цикл
    // передаем серво позицию, которую надо выставить
    myservo.write(pos);

    // решаем прибавить или уменьшить градус поворота головы
    if (from < to) {
      pos += 1;
    } else {
      pos -= 1;
    }

    delay(1);
  }

  checkDistance();
}

void goFromRigthToCenter() {
 moveFromTo(RIGHT_POS, CENTER_POS);
}

void goFromCenterToLeft() {
 moveFromTo(CENTER_POS, LEFT_POS);
}

void goFromLeftToCenter() {
  moveFromTo(LEFT_POS, CENTER_POS);
}

void goFromCenterToRight() {
  moveFromTo(CENTER_POS, RIGHT_POS);
}

void checkDistance(){
  long distance = sr04.Distance();
  Serial.print(distance);
  Serial.println("cm");
}

void loop() {
  if (irrecv.decode(&results)) {
    codeButtonPushed = String(results.value);

    if (codeButtonPushed != "0") {    
      if (codeButtonPushed == left) {
         lastButtonPushedSymbol = "<";
         
         moveFromTo(pos, pos + 10);
      } else if (codeButtonPushed == right) {
         lastButtonPushedSymbol = ">";

         moveFromTo(pos, pos - 10);
      }
    
      if (codeButtonPushed == codeHold) {
          Serial.println("hold " + lastButtonPushedSymbol);

          if (lastButtonPushedSymbol == ">")
          {
            moveFromTo(pos, pos - 10);
          } else if (lastButtonPushedSymbol == "<") {
            moveFromTo(pos, pos + 10);
          }
      } else {
          Serial.println(lastButtonPushedSymbol);
      }
    
      //Serial.println(" (" + String(results.value) + ")");
    }

    irrecv.resume(); // Receive the next value
  } else if (random(1, 1001) == 39) {
    checkDistance();
  }
}
