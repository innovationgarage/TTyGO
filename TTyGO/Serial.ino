State current_state = (State) &initial_state;

class SerialTask : public Task {
  protected:
    void setup()
    {
      const char *initial = (const char *) S(TERMINAL_INIT_BANNER);
      for (int i = 0; i < S_len(initial); i++) {
        current_state = (State) current_state(S_get(initial, i));
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
