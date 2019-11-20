// Sets the terminal cursor to the proper col and row, based on 1
void terminal_setcursor(int col, int row)
{
  u8g2.setCursor(display_width_offset + char_width * (col - 1), display_height_offset + char_height * row);
}

// Draws terminal to the lcd
void terminal_draw()
{
  TERM_DIRTY_SET(current_cursor.x, current_cursor.y, 1);
  bool blink_cursor_tick = (millis() % TERMINAL_CURSOR_BLINK_SPEED_LOOP) > TERMINAL_CURSOR_BLINK_SPEED_ON;
  for (int x = 1; x <= terminal_width; x++)
  {
    for (int y = 1; y <= terminal_height; y++)
    {
      if (!TERM_DIRTY(x, y)) continue;

      // Blinking cursor
      bool blink_cursor_inverse = blink_cursor_tick && x == current_cursor.x && y == current_cursor.y;

      terminal_setcursor(x, y);

      u8g2.setDrawColor(!blink_cursor_inverse);
      char tmp = TERM(x, y).a;
      u8g2.print(tmp == NUL ? ' ' : tmp);

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
    buffer_serial();
  }
  memset(terminal_buffer_dirty, 0, sizeof(terminal_buffer_dirty));
  TERM_DIRTY_SET(current_cursor.x, current_cursor.y, 1);
}

// Draws the screen
void screen_draw()
{
#define APPEND(a, b) a ## b
#define EVAL(a, b) APPEND(a, b)
#define DISPLAY_BUFFER_1 1
#define DISPLAY_BUFFER_2 2
#define DISPLAY_BUFFER_F 3
#if EVAL(DISPLAY_BUFFER_, DISPLAY_BUFFER) == 3
  terminal_draw();
#ifdef ON_SCREEN_KEYBOARD
  if (osk_visible) osk_draw();
#endif

  for (uint8_t y = 0; y < u8g2.getBufferTileHeight(); y++) {
    u8g2.updateDisplayArea(0, y, u8g2.getBufferTileWidth(), 1);
    
    buffer_serial();
  }
#else
  u8g2.firstPage();
  do {
    terminal_draw();
#ifdef ON_SCREEN_KEYBOARD
    if (osk_visible) osk_draw();
#endif

    buffer_serial();
  } while (u8g2.nextPage());
#endif
}

class ScreenTask : public Task {
  protected:
    void setup()
    {
      // init display
      u8g2.begin();
      u8g2.enableUTF8Print();
      u8g2.setFont(LCD_FONT);

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

      screen_draw();

      // Keep the framerate constant
      delay((1000 / LCD_FPS_TARGET) - (millis() - redraw_start));
    }

  private:
    unsigned long next_redraw;
} screen_task;
