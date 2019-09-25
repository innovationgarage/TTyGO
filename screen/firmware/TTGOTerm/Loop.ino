State current_state = (State) &initial_state;

void loop(void) {
  /* Nothing here, check Setup.ino */
}

class TerminalTask : public Task {
  protected:
    void setup()
    {
      scroll_region.upper = 1;
      scroll_region.lower = terminal_height;

      for (int i = 1; i <= terminal_width; i += 8) {
        BIT_SET(terminal_tab_stops, i, 1);
      }
      
      terminal_clear();
    }

    void loop()
    {
      while (Serial.available())
        current_state = (State) current_state(Serial.read());
    }

  private:
    //uint8_t state;
} terminal_task;
