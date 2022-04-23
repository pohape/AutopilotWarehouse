#include <IRremote.h>
 int RECV_PIN = 11;
 IRrecv irrecv(RECV_PIN);
 decode_results results;

 String codeHold = "4294967295";
 String one = "16753245";
 
 String two = "16736925";
 String three = "16769565";
 String four = "16720605";
 String five = "16712445";
 String six = "16761405";
 String seven = "16769055";
 String eight = "16754775";
 String nine = "16748655";
 String zero = "16750695";

 String star = "16738455";
 String grid = "16756815";
 String ok = "16726215";
 String up = "16718055";
 String down = "16730805";
 String left = "16716015";
 String right = "16734885";

 String codeButtonPushed;
 String lastButtonPushedSymbol;
 
 void setup()
 {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
 }
 
 void loop()
 {
  if (irrecv.decode(&results)) {
    codeButtonPushed = String(results.value);
    
    if (codeButtonPushed == one) {
      lastButtonPushedSymbol = "1";
    } else if (codeButtonPushed == two) {
      lastButtonPushedSymbol = "2";
    } else if (codeButtonPushed == three) {
      lastButtonPushedSymbol = "3";
    } else if (codeButtonPushed == four) {
       lastButtonPushedSymbol = "4";
    } else if (codeButtonPushed == five) {
       lastButtonPushedSymbol = "5";
    } else if (codeButtonPushed == six) {
       lastButtonPushedSymbol = "6";
    } else if (codeButtonPushed == seven) {
       lastButtonPushedSymbol = "7";
    } else if (codeButtonPushed == eight) {
       lastButtonPushedSymbol = "8";
    } else if (codeButtonPushed == nine) {
       lastButtonPushedSymbol = "9";
    } else if (codeButtonPushed == zero) {
       lastButtonPushedSymbol = "0";
    } else if (codeButtonPushed == star) {
       lastButtonPushedSymbol = "*";
    } else if (codeButtonPushed == grid) {
       lastButtonPushedSymbol = "#";
    } else if (codeButtonPushed == left) {
       lastButtonPushedSymbol = "<";
    } else if (codeButtonPushed == right) {
       lastButtonPushedSymbol = ">";
    } else if (codeButtonPushed == up) {
       lastButtonPushedSymbol = "/\\";
    } else if (codeButtonPushed == down) {
       lastButtonPushedSymbol = "\\/";
    } else if (codeButtonPushed == ok) {
       lastButtonPushedSymbol = "OK";
    } 
    
    if (codeButtonPushed == codeHold) {
        Serial.print("hold " + lastButtonPushedSymbol);
    } else {
        Serial.print(lastButtonPushedSymbol);
    }
 
    Serial.println(" (" + String(results.value) + ")");
    irrecv.resume(); // Receive the next value
  }
 }
