OneButton button_left(BTN_LEFT, true), button_middle(BTN_MID, true), button_right(BTN_RIGHT, true);

// UP LEFT ENTER ESC DOWN RIGHT
char buttons[6][BUTTON_STRLEN];

void reset_buttons()
{
  for (int i=0; i < 6; i++)
  {
    reset_button(i);
  }
}

void reset_button(int i)
{
  #ifdef FLASH_STRINGS
    const __FlashStringHelper *p;
  #else
    const char *p;
  #endif
  switch (i)
  {
    case 0: p = S("\x1b[A"); break;
    case 1: p = S("\x1b[C"); break;
    case 2: p = S("\n"); break;
    case 3: p = S("\x1b"); break;
    case 4: p = S("\x1b[B"); break;
    case 5: p = S("\x1b[D"); break;
  }
  #ifdef FLASH_STRINGS
    strcpy_P(buttons[i], (PGM_P) p);
  #else
    strcpy(buttons[i], p);
  #endif
}

void button_left_click()
{
  Serial.print(buttons[0]);
}

void button_left_hold()
{
  Serial.print(buttons[1]);
}

// Enter
void button_middle_click()
{
  Serial.print(buttons[2]);
}

// ESC
void button_middle_hold()
{
  Serial.print(buttons[3]);
}

// Cursor down
void button_right_click()
{
  Serial.print(buttons[4]);
}

// Cursor right
void button_right_hold()
{
  Serial.print(buttons[5]);
}

// Task for the buttons
class KeyboardTask : public Task {
  protected:
    void setup()
    {
      reset_buttons();
      // Set buttons (all supported modes: https://github.com/mathertel/OneButton/blob/master/examples/TwoButtons/TwoButtons.ino )
      button_left.attachClick(button_left_click);
      button_left.attachLongPressStart(button_left_hold);
      button_middle.attachClick(button_middle_click);
      button_middle.attachLongPressStart(button_middle_hold);
      button_right.attachClick(button_right_click);
      button_right.attachLongPressStart(button_right_hold);
    }

    void loop()
    {
      // Check buttons
      button_left.tick();
      button_middle.tick();
      button_right.tick();
      delay(10);
    }
} keyboard_task;
