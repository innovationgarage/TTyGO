void setup(void)
{
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(LCD_CLK, LCD_SDA);
  Serial.begin(9600);         // Start the serial port.

  Scheduler.start(&terminal_task);
  Scheduler.start(&keyboard_task);
  Scheduler.start(&screen_task);
  Scheduler.begin();
}
