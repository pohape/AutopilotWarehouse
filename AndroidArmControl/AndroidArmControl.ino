#include <Servo.h>
Servo servoMain;  // create servo object to control a servo
Servo servoRight;
Servo servoLeft;
Servo servoClaw;
int posMain=80, posRight=60, posLeft=130, posClaw=0;

const int ledPin = 13; // Built in LED in Arduino board
String msg,cmd;



void setup()
{  
  // Initialization
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Communication rate of the Bluetooth Module
  msg = "";
  
  servoMain.attach(A1);  // attaches the servo on pin 9 to the servo object
  servoRight.attach(A0);
  servoLeft.attach(6);
  servoClaw.attach(9);
  
  servoMain.write(posMain);
  delay(1000);
  servoRight.write(posRight);
  servoLeft.write(posLeft);
  servoClaw.write(posClaw);
  delay(1500);
}

void blinking(int count)
{  
    for (int i=1; i <= count; i++){
      digitalWrite(ledPin, HIGH); // Turn on LED
      delay(250);
      digitalWrite(ledPin, LOW); // Turn off LED
      delay(250);
   }

}

void turnRight() {
  Serial.println("Turn right\n");
  
  for(posMain;posMain>0;posMain--)
  {
    servoMain.write(posMain);
    delay(5);      // delay 5ms（used to adjust the servo speed）
  }

  delay(1000);
}

void turnLeft() {
  Serial.println("Turn right\n");
  
  for(posMain;posMain<180;posMain++)
  {
    servoMain.write(posMain);
    delay(5);
  }
  
  delay(1000);
}

void openClaw() {
  Serial.println("Open claw\n");
  
  for(posClaw;posClaw<100;posClaw++)
  {
   servoClaw.write(posClaw);
  }

  delay(1000);
}

void closeClaw() {
  Serial.println("Close claw\n");
  
  for(posClaw;posClaw>0;posClaw--)
  {
    servoClaw.write(posClaw);
  }

  delay(1000);
}

void liftUp(){
  Serial.println("Lift up\n");
  
  for(posLeft;posLeft<120;posLeft++)
  {
    servoLeft.write(posLeft);
    delay(5);
  }
  
  delay(1000);
}

void liftDown() {
  Serial.println("Lift down\n");
  
  for(posLeft;posLeft>50;posLeft--)
  {
    servoLeft.write(posLeft);
    delay(5);
  }
  delay(1000);
}

void leftServo5(){
  // left servo rotates to 5 degrees
  Serial.println("left servo rotates to 5 degrees\n");
  
  for(posLeft;posLeft>50;posLeft--)
  {
    servoLeft.write(posLeft);
    delay(5);
  }
  
  delay(1500);
}

void rightServo100(){
    // right servo rotates to 100 degrees
  for(posRight;posRight>50;posRight--)
  {
    servoRight.write(posRight);
    delay(5);
  }
}

void leftServo120(){
// left servo rotates to100 degrees, rocker arm lifts.
  Serial.println("left servo rotates to100 degrees, rocker arm lifts\n");
  for(posLeft;posLeft<120;posLeft++)
  {
    servoLeft.write(posLeft);
    delay(5);
  }
  delay(1000);
}


void loop() {
  if (Serial.available() > 0){ // Check if there is data coming
    msg = Serial.readString(); // Read the message as String
    Serial.println("Android Command: " + msg);
  
    if (msg == "U"){
  
    } else if (msg == "D"){
  
    } else if (msg == "O"){
      openClaw();
    } else if (msg == "C"){
      closeClaw();
    } else if (msg == "F"){
      rightServo100();
      leftServo5();
    } else if (msg == "R"){
      turnRight();
    } else if (msg == "L"){
      turnLeft();
    } else if (msg == "B"){
      leftServo120();
    } 
  }

  msg = ""; // reset command
  
  //turnRight();
  //openClaw();
//  rightServo100();
//  leftServo5();
  
  //closeClaw();
  
//  // left servo rotates to100 degrees, rocker arm lifts.
//  Serial.println("left servo rotates to100 degrees, rocker arm lifts\n");
//  for(posLeft;posLeft<120;posLeft++)
//  {
//    servoLeft.write(posLeft);
//    delay(5);
//  }
//  delay(1000);

  //turnLeft();

  //liftDown();

  //openClaw();

  //liftUp();

  //closeClaw();
}
