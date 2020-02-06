void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);  
  delay(5000);

}

void reset_motion() {
  analogWrite(4, 0);
  analogWrite(5, 150);
}

void do_squeezing() {
  analogWrite(4, 0);
  analogWrite(5, 150);
  delay(3000);
  analogWrite(4, 0);
  analogWrite(5, 0);
  delay(1000);
  analogWrite(4, 150);
  analogWrite(5, 0);
  delay(650);
  analogWrite(4, 0);
  analogWrite(5, 0);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //reset_motion();
  do_squeezing();
}
