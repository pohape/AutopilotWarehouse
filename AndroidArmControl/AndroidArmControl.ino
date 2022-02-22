#include <Servo.h>
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo servoClaw;
int pos1=80, pos2=60, pos3=130, posClaw=0;

const int ledPin = 13; // Built in LED in Arduino board
String msg,cmd;



void setup()
{  
  // Initialization
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Communication rate of the Bluetooth Module
  msg = "";
  
  myservo1.attach(A1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(A0);
  myservo3.attach(6);
  servoClaw.attach(9);
  
  myservo1.write(pos1);
  delay(1000);
  myservo2.write(pos2);
  myservo3.write(pos3);
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
  
  for(pos1;pos1>0;pos1--)
  {
    myservo1.write(pos1);
    delay(5);      // delay 5ms（used to adjust the servo speed）
  }

  delay(1000);
}

void turnLeft() {
  Serial.println("Turn right\n");
  
  for(pos1;pos1<180;pos1++)
  {
    myservo1.write(pos1);
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
  
  for(pos3;pos3<120;pos3++)
  {
    myservo3.write(pos3);
    delay(5);
  }
  
  delay(1000);
}

void liftDown() {
  Serial.println("Lift down\n");
  
  for(pos3;pos3>50;pos3--)
  {
    myservo3.write(pos3);
    delay(5);
  }
  delay(1000);
}


void loop() {
  turnRight();
  openClaw();
  
  // right servo rotates to 100 degrees
  for(pos2;pos2>50;pos2--)
  {
    myservo2.write(pos2);
    delay(5);
  }
  // left servo rotates to 5 degrees
  Serial.println("left servo rotates to 5 degrees\n");
  for(pos3;pos3>50;pos3--)
  {
    myservo3.write(pos3);
    delay(5);
  }
  delay(1500);
  
  closeClaw();
  
  // left servo rotates to100 degrees, rocker arm lifts.
  Serial.println("left servo rotates to100 degrees, rocker arm lifts\n");
  for(pos3;pos3<120;pos3++)
  {
    myservo3.write(pos3);
    delay(5);
  }
  delay(1000);

  turnLeft();

  liftDown();

  openClaw();

  liftUp();

  closeClaw();
}
