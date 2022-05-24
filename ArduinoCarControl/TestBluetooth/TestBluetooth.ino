// For testing purposes use Android app from this repository, located in DroduinoBluetoothConnection directory
#include <SoftwareSerial.h>
const int ledPin = 13; // Built in LED in Arduino board
String command = ""; // Stores response of the HC-06 Bluetooth device
SoftwareSerial BTSerial(53, 51); // RX, TX

void setup()
{  
  delay(500);
  Serial.begin(9600);
  Serial.println("Arduino with HC-06 is ready");

  // HC-06 default baud rate is 9600
  BTSerial.begin(9600);  
  Serial.println("BTserial started at 9600");
  delay(500);
}


void loop() {
   // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  
  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
  }
}
