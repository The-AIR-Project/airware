#include <RotaryEncoder.h>

#define MOTOR_STOP_THRESH 50

RotaryEncoder encoder(2, 3);
// Connect BOUT_1 on the motor driver to the red wire on the 
// motor
uint8_t motor_out_a = 4;
uint8_t motor_out_b = 5;
uint8_t motor_enable = 6;
float Kp = 1;
uint8_t motor_stop_count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motor_out_a, OUTPUT);
  pinMode(motor_out_b, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), encoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoderTick, CHANGE);
}

void encoderTick() {
  encoder.tick();
}

void drive_cw(uint8_t power) {
//  analogWrite(motor_enable, power);
  analogWrite(motor_out_a, LOW);
  analogWrite(motor_out_b, power);
}

void drive_ccw(uint8_t power) {
  analogWrite(motor_out_a, power);
  analogWrite(motor_out_b, LOW);
}

void stop_motor() {
  analogWrite(motor_out_a, LOW);
  analogWrite(motor_out_b, LOW);
}

bool drive_motor(int dest, int start_t) {
  // dest is in encoder counts
  int enc_pos = encoder.getPosition();
  
  int error = dest - enc_pos;
  int u = error * Kp;
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(dest);
  Serial.print(" ");
  Serial.print(enc_pos);
  Serial.println();
  
  if (u < 0) {
    // need to go CCW
    drive_ccw(bound(-1*u, 50, 255));
  }
  else {
    // need to go CW
    drive_cw(bound(u, 50, 255));
  }
  if (abs(u) < 10 || millis() - start_t > 5000) {
    motor_stop_count += 1;
    if (motor_stop_count >= MOTOR_STOP_THRESH) {
      stop_motor();
      return false;
    }
  }
  else {
    motor_stop_count = 0;
    return true;
  }
}

int bound(float x, int low, int high) {
  return (int)(x < low ? low : (x > high ? high : x));
}

void loop() {
  // Right now, drives the motors back and fourth once every 5 seconds.
  int start_t = millis();
  while ((drive_motor(500, start_t) || millis() - start_t < 5000)) {}
  //delay(2000);
  start_t = millis();
  while (drive_motor(0, start_t) || millis() - start_t < 5000) {}
  //delay(2000);
}
