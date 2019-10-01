State current_state = (State) &initial_state;

void loop(void) {
  /* Nothing here, check Setup.ino */
}

char initial[] = TERMINAL_INIT_BANNER;

class TerminalTask : public Task {
  protected:
    void setup()
    {
      scroll_region.upper = 1;
      scroll_region.lower = terminal_height;

      for (int i = 1; i <= terminal_width; i += 8) {
        BIT_SET(terminal_tab_stops, i, 1);
      }
      
      for (int i = 0; i < strlen(initial); i++) {
        current_state = (State) current_state(initial[i]);
      }
    }

    void loop()
    {
      while (Serial.available())
        current_state = (State) current_state(Serial.read());
    }

  private:
    //uint8_t state;
} terminal_task;
