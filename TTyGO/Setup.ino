void loop(void) {
  /* Nothing here, check Setup.ino */
}

// From: https://github.com/esp8266/Arduino/issues/644#issuecomment-246164948
void stopWiFiAndSleep()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setup(void)
{
#if TURN_OFF_WIFI_STARTUP > 0
  stopWiFiAndSleep();
#endif

#ifdef ESPI2C
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(LCD_CLK, LCD_SDA);
#endif

  /* Start the serial port already here so that debug prints work even
     during startup of non-serial tasks... */
  Serial.begin(SERIAL_BAUDS);

  Scheduler.start(&screen_task);
  Scheduler.start(&keyboard_task);
  Scheduler.start(&serial_task);
  Scheduler.begin();
}
