#define BUTTON(pin, active) OneButton(pin, active),
OneButton phys_buttons[] = {BUTTONS};
#undef BUTTON
const size_t phys_buttons_nr = sizeof(phys_buttons) / sizeof(OneButton);

// Button bindings
const size_t buttons_nr = phys_buttons_nr * 2;
char buttons[buttons_nr][BUTTON_STRLEN];

// OneButton does not let us use the same callback function for all
// buttons and just send the button index as a parameter.
// So we need to generate buttons_nr unique callback functions and
// stick their pointers in an array. For this we use a trick described
// here:
// http://blog.aaronballman.com/2011/12/stupid-compiler-tricks/
callbackFunction button_callbacks[buttons_nr];
class ButtonCallback {
public:
  ButtonCallback(int id, void (*fp)( void )) {
    button_callbacks[id] = fp;
  }
};

enum { BUTTON_COUNTER_BASE = __COUNTER__ + 1};

#define BUTTON_DEF1(ctr) void button_ ## ctr ## _callback() { Serial.print(buttons[ctr - BUTTON_COUNTER_BASE]); Serial.flush(); } \
ButtonCallback bc ## ctr = ButtonCallback(ctr - BUTTON_COUNTER_BASE, &button_ ## ctr ## _callback);

#define BUTTON_DEF(ctr) BUTTON_DEF1(ctr)
#define BUTTON(pin, active) BUTTON_DEF(__COUNTER__)
// Twice, as each button has both a click and hold callback
BUTTONS
BUTTONS
#undef BUTTON


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
  for (size_t i = 0; i < phys_buttons_nr; i++) {
    phys_buttons[i].attachClick(button_callbacks[i * 2]);
    phys_buttons[i].attachLongPressStart(button_callbacks[i * 2 + 1]);
  }
  #ifdef ON_SCREEN_KEYBOARD
    phys_buttons[1].attachLongPressStart(&osk_show);
  #endif
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
