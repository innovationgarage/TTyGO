void setup(void) {
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(5, 4);
  // init display

  /* u8g2.begin();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10,"Hello...");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display*/

  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clear();
  for (int i = 0; i < 5; i++)
    u8x8.print(".");
  delay(100);

  delay(300);

  terminal_setup();
}
