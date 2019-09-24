State current_state = (State) &initial_state;

// Main routine
void terminal_loop()
{
  bool lcd_dirty = true; // invoke a redraw
  while (Serial.available())
  {
    current_state = (State) current_state(Serial.read());
  }
  if (lcd_dirty)
    terminal_draw();
  delayMicroseconds(100000);
}


void loop(void) 
{
  terminal_loop();

  // Check buttons
  button_left.isClick();
  button_middle.isClick();
  button_right.isClick();
}
