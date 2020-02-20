#include <RotaryEncoder.h>

#define MOTOR_STOP_THRESH 50
#define ERR_WINDOW 15
#define ENC_OFFSET 780
#define HOME_SPEED 60

RotaryEncoder encoder(2, 3);
// Connect BOUT_1 on the motor driver to the red wire on the 
// motor
uint8_t motor_out_a = 4;
uint8_t motor_out_b = 5;
uint8_t motor_enable = 6;
int16_t errors[ERR_WINDOW] = {0};
int32_t error_sum = 0;
uint8_t eIndex = 0;
unsigned long time_deltas[ERR_WINDOW] = {0};
unsigned long prev_time = 0;
double Kp = 1;
double Ki = 0;
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

bool drive_motor(int32_t dest, int32_t start_t, uint16_t max_cw, uint16_t max_ccw) {
  // dest is in encoder counts
  int16_t enc_pos = encoder.getPosition();
  errors[eIndex] = dest - enc_pos;
  
  
  int32_t error_sum = 0;
  for (uint8_t i = 0; i < ERR_WINDOW; ++i) {
    error_sum += errors[i];
  }
  
  int32_t u = errors[eIndex] * Kp + error_sum * Ki;
  Serial.print(errors[eIndex]);
  Serial.print(" ");
  Serial.print(error_sum);

  Serial.print(" ");
  Serial.print(u);
  Serial.print(" ");
  Serial.print(enc_pos);
  Serial.println();

  eIndex = (eIndex + 1) % ERR_WINDOW;
  
  if (u < 0) {
    // need to go CCW
    drive_ccw(bound(-1*u, 50, max_ccw));
  }
  else {
    // need to go CW
    drive_cw(bound(u, 50, max_cw));
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

void home_motor() {
  delay(2000);
  drive_cw(HOME_SPEED);
  delay(5000);
  encoder.setPosition(ENC_OFFSET);
  uint32_t start_t = millis();
  while ((drive_motor(0, start_t, HOME_SPEED, HOME_SPEED) || millis() - start_t < 5000)) {}
}

int16_t bound(double x, int16_t low, int16_t high) {
  return (int)(x < low ? low : (x > high ? high : x));
}

void loop() {
//  Kp = 0.1;
//  Ki = 0.1;
//  // Right now, drives the motors back and fourth once every 5 seconds.
//  uint32_t start_t = millis();
//  while ((drive_motor(0, start_t, 70, 70) || millis() - start_t < 5000)) {}
//  //delay(2000);
//  
////   Kp = 0.1;
////   Ki = 0.1;
//  start_t = millis();
//  while (millis() - start_t < 2500) { drive_motor(2000, start_t, 150, 70); }
//  //delay(2000);
  home_motor();

}
