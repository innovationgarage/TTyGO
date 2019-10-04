void loop(void) {
  /* Nothing here, check Setup.ino */
}

void setup(void)
{
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(LCD_CLK, LCD_SDA);

  /* Start the serial port already here so that debug prints work even
     during startup of non-serial tasks... */
  Serial.begin(SERIAL_BAUDS);

  Scheduler.start(&screen_task);
  Scheduler.start(&keyboard_task);
  Scheduler.start(&serial_task);
  Scheduler.begin();
}
