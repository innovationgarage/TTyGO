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

int cursor_left, cursor_top, char_height, char_width,
    terminal_width, terminal_height, display_height_offset, display_width_offset; // This is all set by the terminal_setup based on current font and display size
char terminal_buffer[80 * 80]; // Just a maximum, scrolling is not implemented

// Control sequences and internal buffers
bool is_on_command_mode = false, is_on_control_sequence = false;
const int param_temp_buffer_max = 8;
char param_temp_buffer[param_temp_buffer_max];
int param_temp_buffer_pos = 0;

const int control_sequence_param_max = 3;
int control_sequence_param[control_sequence_param_max];
int control_sequence_param_pos = 0;

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
void terminal_clear(int mode = 2)
{
  int cursor_position = (cursor_left - 1) + ((cursor_top - 1) * terminal_width);
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
    cursor_left = 1;
    cursor_top = 1;
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
      cursor_left = 1; cursor_top++; // Line feed
      break;

    default:
      terminal_buffer[(cursor_left - 1) + (cursor_top - 1)*terminal_width] = c; // Put the char and advance
      ++cursor_left;
  }
}

// Parses a numeric parameter from the param buffer
void param_temp_buffer_digest(int default_value = 1)
{
  if (param_temp_buffer_pos > 0)
    control_sequence_param[control_sequence_param_pos] = atoi(param_temp_buffer); // What happens in case of error?
  else
    control_sequence_param[control_sequence_param_pos] = default_value;

  control_sequence_param_pos++;
  param_temp_buffer_pos = 0;
}

// Feeds a char to the param buffer
void param_temp_buffer_eat(char c)
{
  param_temp_buffer[param_temp_buffer_pos++] = c;
  param_temp_buffer[param_temp_buffer_pos] = NUL;
}

// Exits all special modes and resets all buffer positions
void exit_all_command_control_modes()
{
  is_on_command_mode = false;
  is_on_control_sequence = false;
  control_sequence_param_pos = 0;
  param_temp_buffer_pos = 0;
}

// Main routine
void terminal_loop()
{
  bool lcd_dirty = true; // invoke a redraw
  while (Serial.available())
  {
    char c = Serial.read();

    switch (c)
    {
      case ESC:
        is_on_command_mode = true;
        is_on_control_sequence = false;
        lcd_dirty = false;
        break;

      default:
        if (is_on_control_sequence)
        {
          if (c == '-' || (c >= '0' && c <= '9'))
            param_temp_buffer_eat(c);
          else
            switch (c)
            {
              case ';':
                param_temp_buffer_digest(); // Prepares for next parameter... or control code
                break;

              case 'P': // Other control seqs https://www.xfree86.org/current/ctlseqs.html
                param_temp_buffer_digest();
                switch (control_sequence_param[0])
                {
                  case 1:
                    switch (control_sequence_param[1])
                    {
                      case 8: // Report the size of the text area in characters as CSI 8 ; height ; width t
                      case 9: // Report the size of the screen in characters as CSI 9 ; height ; width t
                        Serial.print("[");
                        Serial.print(terminal_height);
                        Serial.print(";");
                        Serial.print(terminal_width);
                        Serial.print("t");
                    }
                    break;
                }
                break;

              case 'J': // Clear screen
                param_temp_buffer_digest(0);
                terminal_clear(control_sequence_param[0]);
                exit_all_command_control_modes();
                break;

              default: // For A,B,C,D,E,F,G,H
                param_temp_buffer_digest();

                // Absolute cursor pos
                if (c == 'H')
                {
                  cursor_top =  control_sequence_param[0];
                  cursor_left = control_sequence_param_pos >= 1 ? control_sequence_param[1] : 1;
                }
                else
                {
                  // Line based (E,F)
                  if (c == 'E' || c == 'F')
                  {
                    cursor_left = 1;
                    cursor_top += (c == 'E' ? 1 : -1) * control_sequence_param[0];
                  }
                  else
                  {
                    cursor_left = (c == 'G') ? control_sequence_param[0] : cursor_left;

                    // Relative
                    cursor_left += ((c == 'C' || c == 'D') ? ((c == 'D' ? -1 : 1) * control_sequence_param[0]) : 0);
                    cursor_top += ((c == 'A' || c == 'B') ? ((c == 'A' ? -1 : 1) * control_sequence_param[0]) : 0);
                  }
                }
                exit_all_command_control_modes();
                break;
            }
        }
        else if (is_on_command_mode)
        {
          switch (c)
          {
            case 'c': // Reset to initial state
              terminal_clear();
              is_on_command_mode = false; // end of command mode
              break;

            case CSI:
              is_on_control_sequence = true; // control sequence
              break;

            default:
              is_on_command_mode = false; // end of command mode
              break;
          }
        }
        else
          terminal_put(c); // Just print the character
        break;
    }

  }

  if (lcd_dirty)
    terminal_draw();
}
