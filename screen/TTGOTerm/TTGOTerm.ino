#include <U8g2lib.h>
#include <Wire.h>
#include "ButtonKing.h"

#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

#define LCD_CLK 5
#define LCD_SDA 4

#define SERIAL_BAUDS 9600

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall 
#define lcd_font u8g2_font_4x6_mf

// ASCII control characters recognised
const char CR = '\r', LF = '\n', VT = '\v', BS = '\b', FF = '\f', ESC = '\e', NUL = '\0', CSI = '[';

ButtonKing button_left(BTN_LEFT, true), button_middle(BTN_MID, true), button_right(BTN_RIGHT, true);
