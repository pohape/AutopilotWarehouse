
#include <Servo.h>
#include "SR04.h"

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


void setup() {
  Serial.begin(9600); // speed for console
  
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
  delay(500); 
}

void goFromRigthToCenter() {
  for (pos = RIGHT_POS; pos <= CENTER_POS; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);
    int ostatDelen = pos % 10;

    if(ostatDelen == 0){
      checkDistance(); 
    } else{              
      delay(15);
    }                
  }
}

void goFromCenterToLeft() {
  for (pos = CENTER_POS; pos <= LEFT_POS; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);
    int ostatDelen = pos % 10;

    if(ostatDelen == 0){
      checkDistance(); 
    } else{              
      delay(15);
    }                         
  }
}

void goFromLeftToCenter() {
  for (pos = LEFT_POS; pos >= CENTER_POS; pos -= 1) { 
    myservo.write(pos);
    int ostatDelen = pos % 10;

    if(ostatDelen == 0){
      checkDistance(); 
    }else{              
      delay(15);
    }                        
  }
}

void goFromCenterToRight() {
  for (pos = CENTER_POS; pos >= RIGHT_POS; pos -= 1) {
    myservo.write(pos);

    int ostatDelen = pos % 10;

    if(ostatDelen == 0){
      checkDistance(); 
    }else{              
      delay(15);
    }                       
  }
}

void checkDistance(){
  long distance = sr04.Distance();
  Serial.print(distance);
  Serial.println("cm");
}

void loop() {  
  goFromCenterToLeft();
  goFromLeftToCenter();
  delay(100);
  goFromCenterToRight();
  goFromRigthToCenter();
  delay(100);
}
