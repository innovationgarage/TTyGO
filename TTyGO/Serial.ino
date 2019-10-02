State current_state = (State) &initial_state;

void loop(void) {
  /* Nothing here, check Setup.ino */
}

char initial[] = TERMINAL_INIT_BANNER;

class SerialTask : public Task {
  protected:
    void setup()
    {
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
} serial_task;
