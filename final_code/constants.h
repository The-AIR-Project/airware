// pin layout
#define P_START_BUTTON 4
#define P_SETTING_BUTTON 5
#define P_CURRENT 0
#define P_VOL_POT 1
#define P_PRESS_POT 2
#define P_RATE_POT 3
#define P_RED_LED 6
#define P_GREEN_LED 8
#define P_SERVO 10

// array indices
#define VOL 0
#define PRES 1
#define RATE 2

// measured physically, was stated to be 270 though
#define MAX_SERVO_POS 250
#define START_POS 0

// these speeds correspond to delays
// i.e higher delay, slower speed
#define MIN_SPEED 10
#define MAX_SPEED 0

// current sensor constants
#define VIN A7
#define VCC 5.0
#define MODEL 2
#define QOV 0.5 * VCC

// potentiometer offset values
#define POT_PIN_OFFSET 1
#define POT_LCD_OFFSET 1

// x-coordinate of numeric values on LCD
#define LCD_H_SPACING 13
