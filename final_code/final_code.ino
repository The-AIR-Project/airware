#include <Servo.h>
#include <LiquidCrystal_I2C.h>
/* 
 *  yellow button D4
 *  black button D5
 *  pot closest to buttons A3
 *  middle pot A2
 *  edge pot A1
 *  current A0
 *  LED 2 (red) D6
 *  LED 1 (green) D8
 *  Servo D10
// 
*/

// all these constants will need to be adjusted
#define MAX_SERVO_POS 250
#define START_POS 250
#define SQUEEZE_POS 40
// these speeds correspond to delays
// i.e higher delay, slower speed
#define MIN_SPEED 20
#define MAX_SPEED 0
#define MAX_POS_ADJ 50
#define BREATH_RATE 5000 // milliseconds per breath 
#define MAX_RATE_ADJ 1000 // milliseconds
// pin layout
#define P_START_BUTTON 4
#define P_SETTING_BUTTON 5
#define P_VOL_POT A1
#define P_PRESS_POT A2
#define P_RATE_POT A3
#define P_RED_LED 6
#define P_GREEN_LED 8
#define P_SERVO 10

#define VIN A7 // define the Arduino pin A0 as voltage input (V in)

double current_data[2][500] = {-100};
int current_index = 0;
const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 2;   // enter the model number (see below)
const int8_t pot_map_radius[3] = {5, 5, 5};
const uint8_t pot_pin_offset = 5; // UPDATE AS NECESSARY
const uint8_t pot_lcd_offset = 1;
const char clear_line[] = "                    ";
const char *setting_titles[3] = {"Volume: ", "Pressure: ", "Breath Rate: "};
const char *age_titles[3] = {"Adult", "Child", "Infant"};
const uint8_t lcd_h_spacing = 13; 
bool ventilating = false;
bool can_read_age_button = true;
bool can_read_start_button = true;
uint8_t pot_vals[3] = {0, 0, 0};
long age_button_time = -100;
uint8_t age_state = 0;
long start_button_time = -100;

 
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
LiquidCrystal_I2C lcd(0x27, 20, 4);

int map_pot_val(uint16_t unmapped_pot_val, int8_t pot_index)
{
  return map(unmapped_pot_val, 0, 1024, pot_map_radius[pot_index], -pot_map_radius[pot_index]);
}

void update_lcd(uint8_t pot_index)
{
  lcd.setCursor(0, pot_index + pot_lcd_offset);
  lcd.print(clear_line);
  lcd.setCursor(0, pot_index + pot_lcd_offset);
  lcd.print(setting_titles[pot_index]);
  lcd.setCursor(lcd_h_spacing, pot_index + pot_lcd_offset);
  lcd.print(pot_vals[pot_index]); // UPDATE THIS TO BE MAPPED VALUE
}

void update_lcd_age(uint8_t age_index)
{
  lcd.setCursor(0,0);
  lcd.print(clear_line);
  lcd.setCursor(0,0);
  lcd.print("Age Setting: ");
  lcd.setCursor(lcd_h_spacing, 0);
  lcd.print(age_titles[age_index]);
}

void check_pot_vals()
{
  for (uint8_t i = 0; i < 3; ++i)
  {
    if (map_pot_val(analogRead(i + pot_pin_offset), i) != pot_vals[i]) 
    {
      pot_vals[i] = map_pot_val(analogRead(i + pot_pin_offset), i);
      update_lcd(i);
    }
  }
}

void check_age_setting()
{
  if (!digitalRead(P_SETTING_BUTTON) && can_read_age_button) 
  {
    age_button_time = millis();
    can_read_age_button = false;
    age_state = (age_state + 1) % 3;
    update_lcd_age(age_state);
  }
}

void check_start_stop()
{
  if (!digitalRead(P_START_BUTTON) && can_read_start_button) 
  {
    start_button_time = millis();
    can_read_start_button = false;
    ventilating = !ventilating;
    // maybe update lcd to say it's started?? also lights??
  }
}

void drive_servo(uint16_t desired_pos)
{
  uint16_t servo_pos = servo.read();
  
  if (servo_pos < desired_pos)
  {
    for (int16_t pos = servo_pos; pos < desired_pos; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      //      int16_t servo_delay = map(analogRead(P_PRESS_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
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
//      int16_t servo_delay = map(analogRead(P_PRESS_POT), 0, 1023, MIN_SPEED, MAX_SPEED);
      int16_t servo_delay = 0;
      delay(servo_delay);
    }
  }
}

int16_t map_servo_pos(int16_t pos)
{
  return map(pos, 0, MAX_SERVO_POS, 0, 180);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(P_VOL_POT, INPUT);
  pinMode(P_RATE_POT, INPUT);
  pinMode(P_PRESS_POT, INPUT);
  pinMode(P_RED_LED, OUTPUT);
  pinMode(P_GREEN_LED, OUTPUT);
  pinMode(P_SETTING_BUTTON, INPUT_PULLUP);
  pinMode(P_START_BUTTON, INPUT_PULLUP);
  servo.attach(P_SERVO);
  servo.write(map_servo_pos(START_POS)); // home the servo
  lcd.init();
  lcd.backlight();
  delay(1500);
}

void loop() {
  check_pot_vals();
  if (!ventilating)
  {
    check_age_setting();
  }
  bool was_ventilating = ventilating;
  check_start_stop();
  if (was_ventilating && !ventilating)
  {
    servo.write(map_servo_pos(START_POS)); // home the servo
  }
  if (ventilating)
  {
    // drive the servos appropriately
  }
  
//  int16_t pos_adj = map(analogRead(P_VOL_POT), 0, 1023, -MAX_POS_ADJ, MAX_POS_ADJ);
////  int16_t dest_pos = map_servo_pos(SQUEEZE_POS + pos_adj);
//  int16_t dest_pos = map_servo_pos(SQUEEZE_POS + pos_adj);
////  int32_t breath_start = millis();
//  drive_servo(dest_pos);
//  delay(500); // needs to be adjusted
//  dest_pos = map_servo_pos(START_POS);
//  drive_servo(dest_pos);
//  drive_servo(START_POS);
////  int32_t breath_end = millis();
////  int16_t time_adj = map(analogRead(P_VOL), 0, 1023, -MAX_RATE_ADJ, MAX_RATE_ADJ);
////  int16_t post_breath_delay = max(0, BREATH_RATE + time_adj - (breath_end - breath_start));
//  int16_t post_breath_delay = 3000;
//  delay(post_breath_delay);
  int16_t pos_adj = map(analogRead(P_VOL_POT), 0, 1023, -MAX_POS_ADJ, MAX_POS_ADJ);
//  int16_t dest_pos = map_servo_pos(SQUEEZE_POS + pos_adj);
  int16_t dest_pos = map_servo_pos(SQUEEZE_POS);
//  int32_t breath_start = millis();
  drive_servo(dest_pos);
  delay(1500);
  drive_servo(map_servo_pos(START_POS));
  int16_t post_breath_delay = 3000;
  delay(post_breath_delay);
}
