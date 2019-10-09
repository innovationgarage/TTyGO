// Draws terminal to the lcd
void terminal_draw()
{
  bool blink_cursor_tick = (millis() % 1000) > 450; 
  for (int x = 1; x <= terminal_width; x++)
    for (int y = 1; y <= terminal_height; y++)
    {
      // Blinking cursor
      bool blink_cursor_inverse = blink_cursor_tick && x == current_cursor.x && y == current_cursor.y;
      terminal_setcursor(x, y);
      
      u8g2.setDrawColor(!blink_cursor_inverse);
      char tmp = TERM(x, y).a;
      u8g2.print(tmp == NUL && blink_cursor_inverse ? ' ' : tmp);
      
#if WIDECHAR > 1
      if (TERM(x, y).b != NUL) u8g2.print(TERM(x, y).b);
#if WIDECHAR > 2
      if (TERM(x, y).c != NUL) u8g2.print(TERM(x, y).c);
#if WIDECHAR > 3
      if (TERM(x, y).d != NUL) u8g2.print(TERM(x, y).d);
#endif
#endif
#endif
      u8g2.setDrawColor(1); // Restore normal drawing color
    }
}

// Draws the screen
void screen_draw()
{
  u8g2.clearBuffer();
  terminal_draw();

  // Other elements on top
  if (osk_visible)
    osk_draw();
  u8g2.sendBuffer();
}

class ScreenTask : public Task {
  protected:
    void setup()
    {
      // init display
      u8g2.begin();
      u8g2.enableUTF8Print();
      u8g2.setFont(lcd_font);

      char_height = u8g2.getMaxCharHeight();
      char_width = u8g2.getMaxCharWidth();
      terminal_height = min(u8g2.getDisplayHeight() / char_height, TERMINAL_MAX_HEIGHT);
      terminal_width = min(u8g2.getDisplayWidth() / char_width, TERMINAL_MAX_WIDTH);
      display_height_offset = (u8g2.getDisplayHeight() - (terminal_height * char_height)) / 2;
      display_width_offset = (u8g2.getDisplayWidth() - (terminal_width * char_width)) / 2;
      //u8g2.sendF("c", 0x0a7); // Invert display
      terminal_reset();
    }

    void loop() {

      unsigned long redraw_start = millis();

      // Refresh the oled
      if (lcd_dirty)
        screen_draw();

      // Keep the framerate constant
      delay((1000 / LCD_FPS_TARGET) - (millis() - redraw_start));
    }

  private:
    unsigned long next_redraw;
} screen_task;
