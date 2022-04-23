int RIGHT_FORWARD=12; // правая сторона вперед, работает в связке с EN_RIGHT, с EN_LEFT ничего не происходит
int RIGHT_BACK=13; // правая сторона назад, работает в связке с EN_RIGHT, с EN_LEFT ничего не происходит
int LEFT_FORWARD=7; // левая сторона вперед, работает в связке с EN_LEFT, с EN_RIGHT ничего не происходит
int LEFT_BACK=8; // левая сторона назад, работает в связке с EN_LEFT, с EN_RIGHT ничего не происходит
int EN_RIGHT=9; // правая сторона
int EN_LEFT=10;// левая сторона

void setup()
{
  //pinMode(RIGHT_FORWARD, OUTPUT);
  //pinMode(RIGHT_BACK, OUTPUT);
  //pinMode(LEFT_FORWARD, OUTPUT);
  //pinMode(LEFT_BACK, OUTPUT);
  //pinMode(EN_RIGHT, OUTPUT);
  //pinMode(EN_LEFT, OUTPUT);

  //digitalWrite(RIGHT_FORWARD,LOW);
  //digitalWrite(RIGHT_BACK,LOW);
  //digitalWrite(LEFT_FORWARD,LOW);
  //digitalWrite(LEFT_BACK,LOW);
  //analogWrite(EN_RIGHT,0);
  //analogWrite(EN_LEFT,0);

  //digitalWrite(RIGHT_FORWARD,HIGH);
//  analogWrite(EN_RIGHT,200);
//  delay(2000);
}
void loop()
{
 digitalWrite(RIGHT_BACK,HIGH);
 analogWrite(EN_RIGHT,200);
 
 delay(2000);

 // pause 2s
 digitalWrite(RIGHT_FORWARD,LOW);
 digitalWrite(RIGHT_BACK,LOW);
 digitalWrite(LEFT_FORWARD,LOW);
 digitalWrite(LEFT_BACK,LOW);
 analogWrite(EN_RIGHT,0);
 analogWrite(EN_LEFT,0);
 delay(2000);

 digitalWrite(RIGHT_FORWARD,HIGH);
 analogWrite(EN_RIGHT,200);

 delay(2000);

 // pause 2s
 digitalWrite(RIGHT_FORWARD,LOW);
 digitalWrite(RIGHT_BACK,LOW);
 digitalWrite(LEFT_FORWARD,LOW);
 digitalWrite(LEFT_BACK,LOW);
 analogWrite(EN_RIGHT,0);
 analogWrite(EN_LEFT,0);
 delay(2000);

 digitalWrite(LEFT_BACK,HIGH);
 analogWrite(EN_LEFT,200);
 
 delay(2000);

 // pause 2s
 digitalWrite(RIGHT_FORWARD,LOW);
 digitalWrite(RIGHT_BACK,LOW);
 digitalWrite(LEFT_FORWARD,LOW);
 digitalWrite(LEFT_BACK,LOW);
 analogWrite(EN_RIGHT,0);
 analogWrite(EN_LEFT,0);
 delay(2000);

 digitalWrite(LEFT_FORWARD,HIGH);
 analogWrite(EN_LEFT,200);

 delay(2000);

 // pause 2s
 digitalWrite(RIGHT_FORWARD,LOW);
 digitalWrite(RIGHT_BACK,LOW);
 digitalWrite(LEFT_FORWARD,LOW);
 digitalWrite(LEFT_BACK,LOW);
 analogWrite(EN_RIGHT,0);
 analogWrite(EN_LEFT,0);
 delay(2000);
  
  // rotate CW
// digitalWrite(RIGHT_FORWARD,LOW);
// digitalWrite(RIGHT_BACK,HIGH);
// analogWrite(EN_RIGHT,200);
// digitalWrite(LEFT_FORWARD,LOW);
// digitalWrite(LEFT_BACK,HIGH);
// analogWrite(EN_LEFT,200);
// delay(2000);
// // pause for 2S
// analogWrite(EN_RIGHT,0);
// analogWrite(EN_LEFT,0);
// delay(1000);
// // rotate CCW
// digitalWrite(RIGHT_FORWARD,HIGH);
// digitalWrite(RIGHT_BACK,LOW);
// analogWrite(EN_RIGHT,100);
// digitalWrite(LEFT_FORWARD,HIGH);
// digitalWrite(LEFT_BACK,LOW);
// analogWrite(EN_LEFT,100);
// delay(2000);
// // pause for 2S
// analogWrite(EN_RIGHT,0);
// analogWrite(EN_LEFT,0);
// delay(2000);
}
