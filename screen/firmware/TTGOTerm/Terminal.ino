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
int tab_size = 4;

ScrollRegion scroll_region;
int char_height, char_width,
    terminal_width, terminal_height, display_height_offset, display_width_offset; // This is all set by the terminal_setup based on current font and display size
char terminal_buffer[80 * 80]; // Just a maximum, scrolling is not implemented


void terminal_scroll(int start, int end, int up) {
  int top = max(scroll_region.upper, start);
  int bottom = min(scroll_region.lower, end);

  if (up == 1) {
    for (int y = top; y <= bottom; y++) {
      for (int x = 1; x <= terminal_width; x++) {
        TERM(x, y) = TERM(x, y + 1);
      }
    }
  } else {
    for (int y = bottom; y >= bottom; y--) {
      for (int x = 1; x <= terminal_width; x++) {
        TERM(x, y) = TERM(x, y - 1);
      }
    }
  }
}

void terminal_scroll_line(int y, int start, int end, int direction_left) {
  int left = max(1, start);
  int right = min(terminal_width, end);

  if (direction_left == 1) {
    for (int x = left; x <= right; x++) {
      TERM(x, y) = TERM(x + 1, y);
    }
  } else {
    for (int x = right; x >= left; x--) {
      TERM(x, y) = TERM(x - 1, y);
    }
  }
}

void terminal_clear_line(int x, int y, int mode) {
  switch (mode)
  {
    case 0:
      for (int i = x; i <= terminal_width; i++) {
        TERM(i, y) = ' ';
      }
      break;
    case 1:
      for (int i = 1; i <= x; i++) {
        TERM(i, y) = ' ';
      }
      break;
    case 2:
      for (int i = 1; i <= terminal_width; i++) {
        TERM(i, y) = ' ';
      }
      break;
  }
}

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
      char c = TERM(x, y);
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
