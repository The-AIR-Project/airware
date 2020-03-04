#include <Servo.h>

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
int pos = 0;

void record_current()
{
  int potentiometer_raw = analogRead(A2);
  float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  float current = voltage / sensitivity[model];// print voltage with 3 decimal places
  current_data[0][current_index] = millis()/1000.0;
  current_data[1][current_index] = current;
  current_index += 1;
  //Serial.println(current);
}

void reset_current_data() {
  for (int i = 0; i < current_index; ++i) {
      if (current_data[0][i] == -100) {
        break;
      }
      Serial.print(current_data[0][i]);
      Serial.print(" ");
      Serial.println(current_data[1][i]);
      current_data[0][i] = -100;
      current_data[1][i] = -100;
    }
    current_index = 0;
}

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  servo.attach(7);
  Serial.begin(9600);
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
  long update_time = 0;
  long start = millis();
  servo.writeMicroseconds(3000);
  while (millis() - start <= 2000) {
    if (abs((long)(update_time - millis())) >= 20)
    {
//      Serial.print("difference: ");
//      Serial.print((long)(update_time - millis()));
//      Serial.print(", update_time: ");
//      Serial.print(update_time);
//      Serial.print(", millis(): ");
//      Serial.println(millis());
      record_current();
      update_time = millis();
    }
  }
  reset_current_data();
  start = millis();
  servo.writeMicroseconds(600);
  while (millis() - start <= 4000) {
    if (abs((long)(update_time - millis())) >= 20)
    {
      record_current();
      update_time = millis();
    }
  }
  reset_current_data();
}
