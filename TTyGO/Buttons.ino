OneButton phys_buttons[] = {OneButton(BTN_LEFT, true), OneButton(BTN_MID, true), OneButton(BTN_RIGHT, true)};
const size_t phys_buttons_nr = sizeof(phys_buttons) / sizeof(OneButton);

// UP LEFT ENTER ESC DOWN RIGHT
const size_t buttons_nr = phys_buttons_nr * 2;
char buttons[buttons_nr][BUTTON_STRLEN];

void reset_buttons()
{
  for (size_t i = 0; i < phys_buttons_nr; i++) {
    phys_buttons[i].setClickTicks(BTN_CLICK_SPEED);
    phys_buttons[i].setPressTicks(BTN_PRESS_SPEED);
    phys_buttons[i].setDebounceTicks(BTN_DEBOUNCE_SPEED);
  }
  for (int i = 0; i < buttons_nr; i++)
    reset_button(i);
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
  #ifdef ON_SCREEN_KEYBOARD
    osk_show();
  #else
    Serial.print(buttons[3]);
  #endif
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

// Just clears all button callbacks
void deatach_buttons()
{
  for (size_t i = 0; i < phys_buttons_nr; i++) {
    phys_buttons[i].attachClick(NULL);
    phys_buttons[i].attachLongPressStart(NULL);
    phys_buttons[i].attachDuringLongPress(NULL);
    phys_buttons[i].attachLongPressStop(NULL);
  }
}

void attach_buttons()
{
  deatach_buttons();

  // Set buttons (all supported modes: https://github.com/mathertel/OneButton/blob/master/examples/TwoButtons/TwoButtons.ino )
  phys_buttons[0].attachClick(button_left_click);
  phys_buttons[0].attachLongPressStart(button_left_hold);
  phys_buttons[1].attachClick(button_middle_click);
  phys_buttons[1].attachLongPressStart(button_middle_hold);
  phys_buttons[2].attachClick(button_right_click);
  phys_buttons[2].attachLongPressStart(button_right_hold);
}

// Task for the buttons
class KeyboardTask : public Task {
  protected:
    void setup()
    {
      reset_buttons();
      attach_buttons();
    }

    void loop()
    {
      // Check buttons
      for (size_t i = 0; i < phys_buttons_nr; i++) {
        phys_buttons[i].tick();
      }
      delay(10);
    }
} keyboard_task;
