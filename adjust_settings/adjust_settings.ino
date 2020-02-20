#include <Servo.h>
// all these constants will need to be adjusted
#define MAX_SERVO_POS 250
#define START_POS 0
#define SQUEEZE_POS 200
// these speeds correspond to delays
// i.e higher delay, slower speed
#define MIN_SPEED 20
#define MAX_SPEED 0
#define MAX_POS_ADJ 50
#define BREATH_RATE 5000 // milliseconds per breath 
#define MAX_RATE_ADJ 1000 // milliseconds
#define SPEED_POT 1
#define VOL_POT 2
#define RATE_POT 3
Servo servo;

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  servo.attach(7);
  Serial.begin(9600);
  pinMode(VOL_POT, INPUT);
  pinMode(RATE_POT, INPUT);
  pinMode(SPEED_POT, INPUT);
  servo.write(START_POS); // home the servo
  delay(1500);
}

void drive_servo(uint16_t desired_pos)
{
  uint16_t servo_pos = servo.read();
  
  if (servo_pos < desired_pos)
  {
    for (int16_t pos = servo_pos; pos < desired_pos; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      
      int16_t servo_delay = map(analogRead(SPEED_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
      delay(servo_delay);
    }
  }
  else 
  {
    for (int16_t pos = servo_pos; pos > desired_pos; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      
      int16_t servo_delay = map(analogRead(SPEED_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
      
      delay(servo_delay);
    }
  }
}

int16_t mapServoPos(int16_t pos)
{
  return map(pos, 0, MAX_SERVO_POS, 0, 180);
}

void loop() {
  int16_t pos_adj = map(analogRead(VOL_POT), 0, 1023, -MAX_POS_ADJ, MAX_POS_ADJ);
  int16_t dest_pos = mapServoPos(SQUEEZE_POS + pos_adj);
  int32_t breath_start = millis();
  drive_servo(dest_pos);
  delay(500); // needs to be adjusted
  dest_pos = mapServoPos(START_POS);
  drive_servo(dest_pos);
  drive_servo(START_POS);
  int32_t breath_end = millis();
  int16_t time_adj = map(analogRead(RATE_POT), 0, 1023, -MAX_RATE_ADJ, MAX_RATE_ADJ);
  int16_t post_breath_delay = max(0, BREATH_RATE + time_adj - (breath_end - breath_start));
  delay(post_breath_delay);
}
