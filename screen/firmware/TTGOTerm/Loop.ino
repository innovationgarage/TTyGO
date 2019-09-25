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
