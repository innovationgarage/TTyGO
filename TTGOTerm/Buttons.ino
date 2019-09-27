OneButton button_left(BTN_LEFT, true), button_middle(BTN_MID, true), button_right(BTN_RIGHT, true);

void button_send_key(char c)
{
  Serial.print(ESC);
  Serial.print(CSI);
  Serial.print(c);
}

// Cursor up
void button_left_click()
{
  button_send_key('A');
}

// Cursor left
void button_left_hold()
{
  button_send_key('C');
}

// Enter
void button_middle_click()
{
  Serial.print(CR);
}

// ESC
void button_middle_hold()
{
  Serial.print(ESC);
}

// Cursor down
void button_right_click()
{
  button_send_key('B');
}

// Cursor right
void button_right_hold()
{
  button_send_key('D');
}

// Task for the buttons
class KeyboardTask : public Task {
  protected:
    void setup()
    {
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
