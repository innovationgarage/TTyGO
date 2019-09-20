#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define lcd_font u8g2_font_4x6_mf 
int lcd_nrows = 8, lcd_ncols = 15;
