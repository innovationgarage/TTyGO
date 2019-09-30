State current_state = (State) &initial_state;

void loop(void) {
  /* Nothing here, check Setup.ino */
}

#ifdef WIDECHAR
  #define WIDECHARSTR " w UTF-8"
#else
  #define WIDECHARSTR ""
#endif

char initial[] = "\x1bc\x1b[1;1HTTGOTerm v. " VERSION "\nVT220" WIDECHARSTR " for Arduino\nBy InnovationGarage AS\n";

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
