/* Terminal emulator

    Implements a simple ANSI escape code terminal interpreter using TTGO OLED ESP8622 SH1106 as platform.
    Uses U8g2lib graphical library, so it is compatible with a lot of displays https://github.com/olikraus/u8g2/wiki

    IMPLEMENTED:

    Escape sequences:

      ESC c Reset to Initial State

    CSI sequences:

      CSI n A
      CSI n B
      CSI n C
      CSI n D
      CSI n E
      CSI n F
      CSI n G
      CSI n ; m H
      CSI n J
      CSI Ps ; Ps ; P for CSI 1 ; 8 ; P and CSI 1 ; 9 ; P (Report the size of the text area)

*/

Cursor current_cursor, saved_cursor;
int char_height, char_width,
    terminal_width, terminal_height, display_height_offset, display_width_offset; // This is all set by the terminal_setup based on current font and display size
char terminal_buffer[80 * 80]; // Just a maximum, scrolling is not implemented

void terminal_cursor_save() {
  saved_cursor = current_cursor;
}

void terminal_cursor_restore() {
  current_cursor = saved_cursor;
}

// Sets the terminal cursor to the proper col and row, based on 1
void terminal_setcursor(int col, int row)
{
  u8g2.setCursor(display_width_offset + char_width * (col - 1), display_height_offset + char_height * row);
}

// Draws terminal to the lcd
void terminal_draw()
{
  u8g2.clearBuffer();
  for (int x = 1; x <= terminal_width; x++)
    for (int y = 1; y <= terminal_height; y++)
    {
      char c = terminal_buffer[(x - 1) + (y - 1) * terminal_width];
      terminal_setcursor(x, y);
      u8g2.print(c);
    }
  u8g2.sendBuffer();
}

// Clears the char buffer
void terminal_clear(int mode)
{
  int cursor_position = (current_cursor.x - 1) + ((current_cursor.y - 1) * terminal_width);
  bool should_clear_next = true;

  for (int i = 0; i < terminal_width * terminal_height; i++)
  {
    switch (mode)
    {
      case 0: // From cursor to the end
        should_clear_next = i >= cursor_position;
        break;

      case 1: // From cursor to beginning
        should_clear_next = i < cursor_position;
        break;
    }

    if (should_clear_next)
      terminal_buffer[i] = NUL;
  }

  if (mode >= 2) // Reposition cursor to the top left
  {
    current_cursor.x = 1;
    current_cursor.y = 1;
  }

  terminal_draw();
}

void terminal_setup()
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

  //u8g2.sendF("c", 0x0a7); // Invert display
  terminal_clear();
}

// Inserts a char into the char buffer
void terminal_put(char c)
{
  switch (c)
  {
    case LF:
      current_cursor.x = 1; current_cursor.y++; // Line feed
      break;

    default:
      terminal_buffer[(current_cursor.x - 1) + (current_cursor.y - 1)*terminal_width] = c; // Put the char and advance
      ++current_cursor.x;
  }
  if (current_cursor.x >= 80) {
    current_cursor.x = 1;
    current_cursor.y++;
  }
  if (current_cursor.y >= 80) {
    current_cursor.y = 1; // Fuck off, we said no scrolling, right? Just don't crash
  }
}

