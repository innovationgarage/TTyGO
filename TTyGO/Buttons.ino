OneButton phys_buttons[] = {OneButton(BTN_LEFT, true), OneButton(BTN_MID, true), OneButton(BTN_RIGHT, true)};
const size_t phys_buttons_nr = sizeof(phys_buttons) / sizeof(OneButton);

// UP LEFT ENTER ESC DOWN RIGHT
char buttons[6][BUTTON_STRLEN];

void reset_buttons()
{
  // Change the tick speed for detecting single clicks
  phys_buttons[0].setClickTicks(BTN_CLICK_SPEED);
  phys_buttons[1].setClickTicks(BTN_CLICK_SPEED);
  phys_buttons[2].setClickTicks(BTN_CLICK_SPEED);

  // Change the tick speed for the long hold detection
  phys_buttons[0].setPressTicks(BTN_PRESS_SPEED);
  phys_buttons[1].setPressTicks(BTN_PRESS_SPEED);
  phys_buttons[2].setPressTicks(BTN_PRESS_SPEED);

  // Change the tick speed for detection of a click as "safe"
  phys_buttons[0].setDebounceTicks(BTN_DEBOUNCE_SPEED);
  phys_buttons[1].setDebounceTicks(BTN_DEBOUNCE_SPEED);
  phys_buttons[2].setDebounceTicks(BTN_DEBOUNCE_SPEED);

  for (int i = 0; i < 6; i++)
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
  phys_buttons[0].attachClick(NULL);
  phys_buttons[0].attachLongPressStart(NULL);
  phys_buttons[0].attachDuringLongPress(NULL);
  phys_buttons[0].attachLongPressStop(NULL);
  phys_buttons[1].attachClick(NULL);
  phys_buttons[1].attachLongPressStart(NULL);
  phys_buttons[0].attachDuringLongPress(NULL);
  phys_buttons[1].attachLongPressStop(NULL);
  phys_buttons[2].attachClick(NULL);
  phys_buttons[2].attachLongPressStart(NULL);
  phys_buttons[2].attachDuringLongPress(NULL);
  phys_buttons[2].attachLongPressStop(NULL);
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
      phys_buttons[0].tick();
      phys_buttons[1].tick();
      phys_buttons[2].tick();
      delay(10);
    }
} keyboard_task;
