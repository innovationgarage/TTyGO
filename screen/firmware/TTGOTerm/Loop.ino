State current_state = (State) &initial_state;

void loop(void) { /* Nothing here, check Setup.ino */ }

class TerminalTask : public Task {
  protected:
    void setup()
    {
      // init display
      u8g2.begin();
      u8g2.setFont(lcd_font);

      char_height = u8g2.getMaxCharHeight();
      char_width = u8g2.getMaxCharWidth();
      terminal_height = u8g2.getDisplayHeight() / char_height;
      terminal_width = u8g2.getDisplayWidth() / char_width;
      display_height_offset = (u8g2.getDisplayHeight() - (terminal_height * char_height)) / 2;
      display_width_offset = (u8g2.getDisplayWidth() - (terminal_width * char_width)) / 2;

      scroll_region.upper = 1;
      scroll_region.lower = terminal_height;

      //u8g2.sendF("c", 0x0a7); // Invert display
      terminal_clear();
    }

    void loop()
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

  private:
    //uint8_t state;
} terminal_task;
