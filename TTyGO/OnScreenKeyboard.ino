#include "utilities.h"

const byte osk_key_size_in_chars = 4, osk_key_code_size_in_chars = 4;

typedef struct {
  char label[osk_key_size_in_chars];
  char code[osk_key_code_size_in_chars];
} Key;

const Key osk_keyboard[] = { {"A", "A"}, {"B", "B"}, {"C", "C"}, {"D", "D"}, {"E", "E"}, {"F", "F"}, {"G", "G"}, {"H", "H"}, {"I", "I"}, {"J", "J"}, {"K", "K"},
  {"L", "L"}, {"M", "M"}, {"N", "N"}, {"O", "O"}, {"P", "P"}, {"Q", "Q"}, {"R", "R"}, {"S", "S"}, {"T", "T"}, {"U", "U"}, {"V", "V"}, {"W", "W"}, {"X", "X"},
  {"Y", "Y"}, {"Z", "Z"}, {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"a", "a"},
  {"b", "b"}, {"c", "c"}, {"d", "d"}, {"e", "e"}, {"f", "f"}, {"g", "g"}, {"h", "h"}, {"i", "i"}, {"j", "j"}, {"k", "k"}, {"l", "l"}, {"m", "m"}, {"n", "n"},
  {"o", "o"}, {"p", "p"}, {"q", "q"}, {"r", "r"}, {"s", "s"}, {"t", "t"}, {"u", "u"}, {"v", "v"}, {"w", "w"}, {"x", "x"}, {"y", "y"}, {"z", "z"}, {"[", "["},
  {"]", "]"}, {"(", "("}, {")", ")"}, {".", "."}, {",", ","}, {":", ":"}, {";", ";"}, {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/", "/"}, {"!", "!"}, {"\"", "\""},
  {"#", "#"}, {"%", "%"}, {"&", "&"}, {"?", "?"}, {"'", "'"}, {"*", "*"}, {"|", "|"}, {"_", "_"}, {"<", "<"}, {">", ">"}, {"ESC", ESC}, {"RET", "\n"}, {"TAB", "\t"},
};

bool osk_visible = false, osk_position_top;
const byte OSK_OPEN = 10, OSK_OPENING = 20, OSK_OPENED = 21, OSK_RUNNING = 30, OSK_CLOSE = 40, OSK_CLOSING = 41, OSK_CLOSED = 50;
const int osk_keyboard_length = ArrayLength(osk_keyboard), osk_default_selection = 0;
int osk_current_selection, osk_animation_frame, osk_position;
double osk_offset_keys = osk_offset_keys_default; // For the bounce animation
long osk_next_hold;
byte osk_current_mode;

void osk_draw_box()
{
  osk_position =  osk_animation_frame / 12;
  u8g2.drawBox(0, osk_position_top ? (0) : (9 - osk_position + u8g2.getDisplayHeight() - (char_height + 2)), u8g2.getDisplayWidth(), osk_position);
}

// Draws the on screen keyboard to the lcd
void osk_draw()
{
  switch (osk_current_mode)
  {
    case OSK_OPEN:
      osk_position_top = current_cursor.y > terminal_height / 2; // OSK goes on top if the cursor is on the bottom
      osk_animation_frame = 0;
      osk_current_mode = OSK_OPENING;
      osk_current_selection = osk_default_selection;

    case OSK_OPENING:
      osk_draw_box();
      if ((osk_animation_frame += 20) > 100)
        osk_current_mode = OSK_OPENED;
      break;

    case OSK_OPENED:
      attach_osk_buttons();
      osk_current_mode = OSK_RUNNING;

    case OSK_RUNNING:
      u8g2.drawBox(0, osk_position_top ? 0 : (u8g2.getDisplayHeight() - (char_height + 2)), u8g2.getDisplayWidth(), 9);
      for (int i = -keys_to_show_per_side; i <= +keys_to_show_per_side; i++)
      {
        u8g2.setCursor(((i + keys_to_show_per_side) * osk_key_size_in_chars * char_width) + osk_offset_keys,
                       osk_position_top ? char_height + 1 : (u8g2.getDisplayHeight() - 2));
        u8g2.setDrawColor(0 == i ? 1 : 0);
        u8g2.print(osk_keyboard[osk_check_bounds(osk_current_selection + i)].label);
        u8g2.setDrawColor(1);
      }

      osk_offset_keys += (osk_offset_keys_default - osk_offset_keys) / osk_offset_bounce_speed;
      if (abs(osk_offset_keys - osk_offset_keys_default) > 0.1)
        lcd_dirty = true;
      break;

    case OSK_CLOSE:
      osk_animation_frame = 100;
      osk_current_mode = OSK_CLOSING;

    case OSK_CLOSING:
      osk_draw_box();
      if ((osk_animation_frame -= 20) <= 0)
        osk_current_mode = OSK_CLOSED;
      break;

    case OSK_CLOSED:
      osk_visible = false;
      attach_buttons();
      break;
  }
}

// Returns corrected item if it was out of bounds
int osk_check_bounds(int pos)
{
  int corrected = pos;

  // Loop boundaries
  if (pos < 0)
    corrected = osk_keyboard_length + pos;
  else if (pos >= osk_keyboard_length)
    corrected = pos - osk_keyboard_length;

  return corrected;
}

// Select a key, if direction is true, it goes forward
bool osk_move_selection(bool direction)
{
  lcd_dirty = true;
  osk_offset_keys = osk_offset_keys_default - (direction ? -1 : 1) * osk_offset_bounce_length;
  osk_current_selection = osk_check_bounds(osk_current_selection + (direction ? 1 : -1));
}

void button_osk_left_click()
{
  osk_move_selection(false);
}

void button_osk_middle_click()
{
  Serial.print(osk_keyboard[osk_current_selection].code);
}

void button_osk_middle_hold()
{
  osk_hide();
}

void button_osk_left_hold_start()
{
  osk_animation_frame = osk_hold_delay;
}

void button_osk_right_hold_start()
{
  osk_animation_frame = osk_hold_delay;
}

void button_osk_right_click()
{
  osk_move_selection(true);
}

// Select a key, while holding, using delay, if direction is true, it goes forward
void osk_move_selection_held(boolean direction)
{
  if (millis() > osk_next_hold)
  {
    if (direction)
      button_osk_right_click();
    else
      button_osk_left_click();

    osk_next_hold = millis() + osk_animation_frame;

    if (osk_animation_frame > 0)
      osk_animation_frame -= osk_hold_delay_acceleration;
  }
}

void button_osk_right_hold()
{
  osk_move_selection_held(true);
}

void button_osk_left_hold()
{
  osk_move_selection_held(false);
}

void attach_osk_buttons()
{
  // Set buttons (all supported modes: https://github.com/mathertel/OneButton/blob/master/examples/TwoButtons/TwoButtons.ino )
  button_left.attachClick(button_osk_left_click);
  button_left.attachLongPressStart(button_osk_left_hold_start);
  button_left.attachDuringLongPress(button_osk_left_hold);
  button_middle.attachClick(button_osk_middle_click);
  button_middle.attachLongPressStart(button_osk_middle_hold);
  button_right.attachClick(button_osk_right_click);
  button_right.attachLongPressStart(button_osk_right_hold_start);
  button_right.attachDuringLongPress(button_osk_right_hold);
}

void osk_show()
{
  osk_current_mode = OSK_OPEN;
  osk_visible = true;
  deatach_buttons ();
}

void osk_hide()
{
  osk_current_mode = OSK_CLOSE;
  deatach_buttons ();
}
