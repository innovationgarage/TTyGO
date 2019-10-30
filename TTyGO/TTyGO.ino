#include <U8g2lib.h>
#include "OneButton.h"

U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);  // Arduboy

// ************************************************
// **** Hardware mapping configuration  ***********
// ************************************************
#define BTN_LEFT A2
#define BTN_DOWN A3
#define BTN_RIGHT A1
#define BTN_UP A0
#define BTN_A 7
#define BTN_B 8


void setup()
{
  u8g2.begin();
  attach_buttons();
}

OneButton button_left(BTN_LEFT, true), button_down(BTN_DOWN, true), button_right(BTN_RIGHT, true), button_up(BTN_UP, true), button_a(BTN_A, true), button_b(BTN_B, true);

int x = 0, y = 15;
void loop()
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(x,y,"Hello Egil");
  } while ( u8g2.nextPage() );

  // Check buttons
  button_left.tick();
  button_down.tick();
  button_right.tick();
  button_up.tick();
  button_a.tick();
  button_b.tick();

  delay(10);
}
