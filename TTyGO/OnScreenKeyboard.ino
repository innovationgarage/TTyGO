#include "utilities.h"
bool osk_visible = false;
const byte OSK_OPEN = 10, OSK_OPENING = 20, OSK_OPENED = 21, OSK_RUNNING = 30, OSK_CLOSE = 40, OSK_CLOSING = 41, OSK_CLOSED = 50;

typedef struct {
  char label[4];
  char code[8];
} Key;

const Key osk_keyboard[] = { {"A", "A"}, {"B", "B"}, {"C", "C"}, {"D", "D"}, {"E", "E"}, {"F", "F"}, {"G", "G"}, {"H", "H"}, {"I", "I"}, {"J", "J"}, {"K", "K"},
  {"L", "L"}, {"M", "M"}, {"N", "N"}, {"O", "O"}, {"P", "P"}, {"Q", "Q"}, {"R", "R"}, {"S", "S"}, {"T", "T"}, {"U", "U"}, {"V", "V"}, {"W", "W"}, {"X", "X"},
  {"Y", "Y"}, {"Z", "Z"}, {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"a", "a"},
  {"b", "b"}, {"c", "c"}, {"d", "d"}, {"e", "e"}, {"f", "f"}, {"g", "g"}, {"h", "h"}, {"i", "i"}, {"j", "j"}, {"k", "k"}, {"l", "l"}, {"m", "m"}, {"n", "n"},
  {"o", "o"}, {"p", "p"}, {"q", "q"}, {"r", "r"}, {"s", "s"}, {"t", "t"}, {"u", "u"}, {"v", "v"}, {"w", "w"}, {"x", "x"}, {"y", "y"}, {"z", "z"}, {"[", "["},
  {"]", "]"}, {"(", "("}, {")", ")"}, {".", "."}, {",", ","}, {":", ":"}, {";", ";"}, {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/", "/"}, {"!", "!"}, {""", """},
  {"#", "#"}, {"%", "%"}, {"&", "&"}, {"?", "?"}, {"'", "'"}, {"*", "*"}, {"|", "|"}, {"_", "_"}, {"<", "<"}, {">", ">"}, {"ESC", ESC}, {"RET", "\n"}, {"TAB", "\t"},
};

const int osk_keyboard_length = ArrayLength(osk_keyboard);
const int osk_default_selection = 0;
const int osk_largejump = 10; // When holding a key
int osk_current_selection, osk_animation_frame;
bool osk_position_top;
byte osk_current_mode;
int osk_position;
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
      //u8g2.drawBox(0, osk_position_top?0:(u8g2.getDisplayHeight()-(char_height+2)), min(1,), char_height+2);
      osk_position = osk_animation_frame / 12;
      u8g2.drawBox(0, osk_position_top ? (0) : (9 - osk_position + u8g2.getDisplayHeight() - (char_height + 2)), u8g2.getDisplayWidth(), osk_position);

      if ((osk_animation_frame += 20) > 100)
        osk_current_mode = OSK_OPENED;
      break;

    case OSK_OPENED:
      attach_osk_buttons();
      osk_current_mode = OSK_RUNNING;

    case OSK_RUNNING:
      u8g2.drawBox(0, osk_position_top ? 0 : (u8g2.getDisplayHeight() - (char_height + 2)), u8g2.getDisplayWidth(), 9);

      u8g2.setCursor(12, osk_position_top ? char_height + 1 : (u8g2.getDisplayHeight() - 2));
      u8g2.setDrawColor(0);
      u8g2.print(osk_keyboard[osk_current_selection].label);
      u8g2.setDrawColor(1);
      break;

    case OSK_CLOSE:
      osk_animation_frame = 100;
      osk_current_mode = OSK_CLOSING;

    case OSK_CLOSING:
      //u8g2.drawBox(0, osk_position_top?0:(u8g2.getDisplayHeight()-(char_height+2)), min(1,), char_height+2);
      osk_position = osk_animation_frame / 12;
      // TODO: Create only one function to draw the keyboard
      u8g2.drawBox(0, osk_position_top ? (0) : (9 - osk_position + u8g2.getDisplayHeight() - (char_height + 2)), u8g2.getDisplayWidth(), osk_position);

      if ((osk_animation_frame -= 20) <= 0)
        osk_current_mode = OSK_CLOSED;
      break;

    case OSK_CLOSED:
      osk_visible = false;
      attach_buttons();
      break;
  }
}

// Returns true if it was out of bounds
bool osk_update_check_bounds()
{
  // Loop boundaries
  if (osk_current_selection < 0)
    osk_current_selection = osk_keyboard_length + osk_current_selection;
  else if (osk_current_selection >= osk_keyboard_length)
    osk_current_selection = osk_current_selection - osk_keyboard_length;
  else
    return false;
  return true;
}

// Select a key
bool osk_update_selection(int v)
{
  lcd_dirty = true;
  osk_current_selection += v;
  while (osk_update_check_bounds());
}

void button_osk_left_click()
{
  osk_update_selection(-1);
}

void button_osk_middle_click()
{
  Serial.print(osk_keyboard[osk_current_selection].code);
}

void button_osk_middle_hold()
{
  osk_hide();
}

void button_osk_right_click()
{
  osk_update_selection(1);
}

void attach_osk_buttons()
{
  // Set buttons (all supported modes: https://github.com/mathertel/OneButton/blob/master/examples/TwoButtons/TwoButtons.ino )
  button_left.attachClick(button_osk_left_click);
  button_left.attachLongPressStart(button_osk_left_click);
  button_left.attachDuringLongPress(button_osk_left_click);

  button_middle.attachClick(button_osk_middle_click);
  button_middle.attachLongPressStart(button_osk_middle_hold);

  button_right.attachClick(button_osk_right_click);
  button_right.attachLongPressStart(button_osk_right_click);
  button_right.attachDuringLongPress(button_osk_right_click);
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
