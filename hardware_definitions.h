

#define DELAY_TIME  500


//*********************** ARDUINO ***********************//
//      MACRO             ARDUINO PIN
//#define LED_RED_PIN       3
#define LED_WHITE_PIN     4

//MACROS PARA BOTONERA ARMADA A MANO CON PULSADORES
//buttons for arduino NANO
//#define BUTTON_ENTER      6     
//#define BUTTON_BACK       7     
//#define BUTTON_UP         A0
//#define BUTTON_RIGHT      A1
//#define BUTTON_DOWN       A2
//#define BUTTON_LEFT       A3

//buttons for arduino UNO  
#define BUTTON_LEFT       A3
#define BUTTON_DOWN       A2
#define BUTTON_RIGHT      A1
#define BUTTON_UP         A0
#define BUTTON_ENTER      4     
#define BUTTON_BACK       3    
//#define BUTTON_LEFT       8
//#define BUTTON_DOWN       9
//#define BUTTON_RIGHT      10
//#define BUTTON_UP         11
//#define BUTTON_ENTER      13     
//#define BUTTON_BACK       12 


//*********************** DISPLAY ***********************//
// Using a display of 4 ROWS x 20 COLS
#define DISPLAY_MAX_ROW       4
#define DISPLAY_MAX_COLS      20
#include <LiquidCrystal_I2C.h>
// Create object lcd with direction: 0x27 and 20 cols by 4 rows
// PCF8574TI -> 0x27
// PCF8574NxP -> 0x3F
// https://cursos.mcielectronics.cl/2022/12/09/interfaz-de-un-lcd-i2c-con-arduino/
LiquidCrystal_I2C lcd(0x27,DISPLAY_MAX_COLS,DISPLAY_MAX_ROW);  //

#define STORE_CHAR_UPARROW      0
#define STORE_CHAR_DOWNARROW    1
#define STORE_CHAR_CHECK        2
#define STORE_CHAR_CROSS        3

//************** Special symbols creation **************//
byte upArrow[8] = {
    0b00100,  //   *
    0b01110,  //  ***
    0b10101,  // * * *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100   //   *
};
byte downArrow[8] = {
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b10101,  // * * *
    0b01110,  //  ***
    0b00100   //   *
};
byte check[] = {
    0b00000,  //   
    0b00000,  //   
    0b00001,  //     *
    0b00011,  //    **
    0b10110,  // * **
    0b11100,  // ***
    0b01000,  //  *
    0b00000   //  
};
byte cross[] = {
    0b00000,  //   
    0b00000,  //   
    0b11011,  // ** **
    0b01010,  //  * *
    0b00100,  //   *
    0b01010,  //  * * 
    0b11011,  // ** **  
    0b00000   //  
};



//*********************** KEYPAD  ***********************//
#include <Keypad.h>
// Using a keypad of 4 ROWS x 4 COLS
//      MACRO             ARDUINO PIN
#define KEYPAD_ROWS_QTY   4
#define KEYPAD_COLS_QTY   4

#define KEYPAD_ROW_0      13
#define KEYPAD_ROW_1      12
#define KEYPAD_ROW_2      11
#define KEYPAD_ROW_3      10
#define KEYPAD_COL_0      9
#define KEYPAD_COL_1      8
#define KEYPAD_COL_2      7
#define KEYPAD_COL_3      6
 
// Define keyboard Keymap
char keys[KEYPAD_ROWS_QTY][KEYPAD_COLS_QTY] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 

// Pins to be used
byte rowPins[KEYPAD_ROWS_QTY] = {KEYPAD_ROW_0, KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3}; 
byte colPins[KEYPAD_COLS_QTY] = {KEYPAD_COL_0, KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3}; 

   
// Create Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS_QTY, KEYPAD_COLS_QTY );

// Configure keyboard keys (ASCII)
//#define SELECT_UP                 // 
//#define SELECT_DOWN               // 
//#define SELECT_LEFT               // 
//#define SELECT_RIGHT              //
//#define SELECT_ENTER              // 
//#define SELECT_BACK               //  
//#define SELECT_UP         50      // Numpad 2
//#define SELECT_DOWN       56      // Numpad 8
//#define SELECT_LEFT       52      // NUMPAD 4
//#define SELECT_RIGHT      54      // NUMPAD 6
//#define SELECT_ENTER      65      // Numpad A
//#define SELECT_BACK       66      // Numpad B
//#define SELECT_CLEAR      67      // Numpad C
//#define SELECT_BACKSPACE  68      // Numpad D

// ASCII
// 48 -> Numpad 0
// 49 -> Numpad 1
// 50 -> Numpad 2
// 51 -> Numpad 3
// 52 -> Numpad 4
// 53 -> Numpad 5
// 54 -> Numpad 6
// 55 -> Numpad 7
// 56 -> Numpad 8
// 57 -> Numpad 9
// 65 -> Numpad A
// 66 -> Numpad B
// 67 -> Numpad C
// 68 -> Numpad D
// 42 -> Numpad *
// 35 -> Numpad #


////////////////////////////////////////////////////////////////////////////////////
////////////////    hardware initialization setup   ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void hardware_init(){
    pinMode(BUTTON_ENTER, INPUT);
    pinMode(BUTTON_BACK, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
  
    pinMode(LED_WHITE_PIN, OUTPUT);
//    pinMode(LED_RED_PIN, OUTPUT);
//    pinMode(LED_BUILTIN, OUTPUT);
  
    // Initialize the LCD
    lcd.init();
    // Turning ON the LCD backlight
    lcd.backlight();
    
    // create special characters
    lcd.createChar(STORE_CHAR_UPARROW, upArrow);
    lcd.createChar(STORE_CHAR_DOWNARROW, downArrow);
//    lcd.createChar(STORE_CHAR_CHECK, check);
//    lcd.createChar(STORE_CHAR_CROSS, cross);
    
};
