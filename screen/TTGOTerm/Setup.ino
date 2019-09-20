void setup(void) {
  // change hardware I2C pins to (5,4) (D1,D2)
  Wire.begin(5, 4);
  Serial.begin(9600);         // Start the serial port.

  terminal_setup();
}
