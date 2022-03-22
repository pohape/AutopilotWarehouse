// For testing purposes use Android app from this repository, located in DroduinoBluetoothConnection directory

const int ledPin = 13; // Built in LED in Arduino board
String msg, cmd;

void setup()
{  
  // Initialization
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Communication rate of the Bluetooth Module
  msg = "";
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

void loop() {
  if (Serial.available() > 0) { // Check if there is data coming
    msg = Serial.readString(); // Read the message as String
    Serial.println("Android Command: " + msg);
  
    if (msg == "U"){
      blinking(2);
    } else if (msg == "D"){
      blinking(3);
    } else if (msg == "O"){
      blinking(4);
    } else if (msg == "C"){
      blinking(5);
    } else if (msg == "F"){
      blinking(6);
    } else if (msg == "R"){
      blinking(7);
    } else if (msg == "L"){
      blinking(9);
    } else if (msg == "B"){
      blinking(10);
    } 
  }

  msg = ""; // reset command
}
