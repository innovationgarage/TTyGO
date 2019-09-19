void loop(void) {
  /*u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(1000); */

  terminal_loop();
}
