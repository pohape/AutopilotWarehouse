const int tracingPinRight = 4;
const int tracingPinCenter = 5;
const int tracingPinLeft = 6;
const int ledPin = 13;

void setup() {
  pinMode(tracingPinRight, INPUT);
  pinMode(tracingPinCenter, INPUT);
  pinMode(tracingPinLeft, INPUT);
  pinMode(ledPin, OUTPUT);
}

void readPinAndControlLed(int tracingPin){

  int val = digitalRead(tracingPin);
  
  if(val == HIGH){   
      digitalWrite(ledPin, HIGH);
  }
  else {
      digitalWrite(ledPin, LOW);
  }
  
}

void loop() {

  readPinAndControlLed(tracingPinRight);
  readPinAndControlLed(tracingPinCenter);
  readPinAndControlLed(tracingPinLeft);
  
}
