#include <Servo.h>

Servo servo;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  servo.attach(7);
}

void loop() {
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(1);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
  delay(3000);
  servo.writeMicroseconds(1000);
  delay(3000);
  servo.writeMicroseconds(2000);
}
