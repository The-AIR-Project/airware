#include <Servo.h>
// all these constants will need to be adjusted
#define MAX_SERVO_POS 250
#define START_POS 30
#define SQUEEZE_POS 250
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

#define VIN A7 // define the Arduino pin A0 as voltage input (V in)

double current_data[2][500] = {-100};
int current_index = 0;
const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 2;   // enter the model number (see below)
 
float cutOffLimit = 0.05;// set the current which below that value, doesn't matter. Or set 0.5
 
/*
          "ACS712ELCTR-05B-T",// for model use 0
          "ACS712ELCTR-20A-T",// for model use 1
          "ACS712ELCTR-30A-T"// for model use 2  
sensitivity array is holding the sensitivy of the  ACS712
current sensors. Do not change. All values are from page 5  of data sheet          
*/
float sensitivity[] ={
          0.185,// for ACS712ELCTR-05B-T
          0.100,// for ACS712ELCTR-20A-T
          0.066// for ACS712ELCTR-30A-T
     
         };
 
 
const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage

Servo servo;

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  servo.attach(10);
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
      //      int16_t servo_delay = map(analogRead(SPEED_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
      int16_t servo_delay = 0;
      delay(servo_delay);
    }
  }
  else 
  {
    int counter = 0;
    for (int16_t pos = servo_pos; pos > desired_pos; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree

      
      servo.write(pos);              // tell servo to go to position in variable 'pos'
//      int16_t servo_delay = map(analogRead(SPEED_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
      int16_t servo_delay = 0;
      delay(servo_delay);
    }
  }
}

int16_t mapServoPos(int16_t pos)
{
  return map(pos, 0, MAX_SERVO_POS, 0, 180);
}

void loop() {
//  int16_t pos_adj = map(analogRead(VOL_POT), 0, 1023, -MAX_POS_ADJ, MAX_POS_ADJ);
////  int16_t dest_pos = mapServoPos(SQUEEZE_POS + pos_adj);
//  int16_t dest_pos = mapServoPos(SQUEEZE_POS + pos_adj);
////  int32_t breath_start = millis();
//  drive_servo(dest_pos);
//  delay(500); // needs to be adjusted
//  dest_pos = mapServoPos(START_POS);
//  drive_servo(dest_pos);
//  drive_servo(START_POS);
////  int32_t breath_end = millis();
////  int16_t time_adj = map(analogRead(RATE_POT), 0, 1023, -MAX_RATE_ADJ, MAX_RATE_ADJ);
////  int16_t post_breath_delay = max(0, BREATH_RATE + time_adj - (breath_end - breath_start));
//  int16_t post_breath_delay = 3000;
//  delay(post_breath_delay);
  drive_servo(mapServoPos(START_POS));
  delay(1500);
  int16_t pos_adj = map(analogRead(VOL_POT), 0, 1023, -MAX_POS_ADJ, MAX_POS_ADJ);
//  int16_t dest_pos = mapServoPos(SQUEEZE_POS + pos_adj);
  int16_t dest_pos = mapServoPos(SQUEEZE_POS);
//  int32_t breath_start = millis();
  drive_servo(dest_pos);
  int16_t post_breath_delay = 3000;
  delay(post_breath_delay);
}
