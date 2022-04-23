const int tracingPinRight = A2;
const int tracingPinCenter = A0;
const int tracingPinLeft = A1;
const int ledPin = 13;

void setup() {
  pinMode(tracingPinRight, INPUT);
  pinMode(tracingPinCenter, INPUT);
  pinMode(tracingPinLeft, INPUT);
  pinMode(ledPin, OUTPUT);
}

void readPinAndControlLed(int tracingPin){
  int val = digitalRead(tracingPin);
  
  if (val == HIGH){   
      digitalWrite(ledPin, HIGH);
  } else {
      digitalWrite(ledPin, LOW);
  }
}

void loop() {
  readPinAndControlLed(tracingPinRight);
  readPinAndControlLed(tracingPinCenter);
  readPinAndControlLed(tracingPinLeft);
}
