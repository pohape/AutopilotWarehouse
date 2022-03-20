
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 90;    // variable to store the servo position

const int RIGHT_POS = 0;
const int CENTER_POS = 90;
const int LEFT_POS = 180;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
  delay(500); 
}

void goFromRigthToCenter() {
  for (pos = RIGHT_POS; pos <= CENTER_POS; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(15);              
  }
}

void goFromCenterToLeft() {
  for (pos = CENTER_POS; pos <= LEFT_POS; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(15);                       
  }
}

void goFromLeftToCenter() {
  for (pos = LEFT_POS; pos >= CENTER_POS; pos -= 1) { 
    myservo.write(pos);             
    delay(15);                      
  }
}

void goFromCenterToRight() {
  for (pos = CENTER_POS; pos >= RIGHT_POS; pos -= 1) {
    myservo.write(pos);              
    delay(15);                       
  }
}

void loop() {
  goFromCenterToLeft();
  goFromLeftToCenter();
  delay(100);
  goFromCenterToRight();
  goFromRigthToCenter();
  delay(100);
}
