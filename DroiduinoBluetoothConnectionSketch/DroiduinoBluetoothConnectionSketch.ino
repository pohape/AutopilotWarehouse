const int ledPin = 13; // Built in LED in Arduino board
String msg,cmd;

void setup() {
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
  while(true) {
    // To read message received from other Bluetooth Device
    if (Serial.available() > 0){ // Check if there is data coming
      msg = Serial.readString(); // Read the message as String
      Serial.println("Android Command: " + msg);
    }
  
    // Control LED in Arduino board
    if (msg == "U"){
      
      Serial.println("UP command: LED 1\n"); // Then send status message to Android
      blinking(1);
      
      msg = ""; // reset command
    } else if (msg == "D"){
      
      Serial.println("DOWN command: LED 2\n"); // Then send status message to Android
      blinking(2);
      
      msg = ""; // reset command
    } else if (msg == "O"){
      
      Serial.println("OPEN command: LED 3\n"); // Then send status message to Android
      blinking(3);
      
      msg = ""; // reset command
    } else if (msg == "C"){
      
      Serial.println("CLOSE command: LED 4\n"); // Then send status message to Android
      blinking(4);
      
      msg = ""; // reset command
    } else if (msg == "F"){
      
      Serial.println("FORWARD command: LED 5\n"); // Then send status message to Android
      blinking(5);
      
      msg = ""; // reset command
    } else if (msg == "R"){
      
      Serial.println("RIGHT command: LED 6\n"); // Then send status message to Android
      blinking(6);
      
      msg = ""; // reset command
    } else if (msg == "L"){
      
      Serial.println("LEFT command: LED 7\n"); // Then send status message to Android
      blinking(7);
      
      msg = ""; // reset command
    } else if (msg == "B"){
      
      Serial.println("BACK command: LED 8\n"); // Then send status message to Android
      blinking(8);
      
      msg = ""; // reset command
    } 

    delay(1); 
  }
}
