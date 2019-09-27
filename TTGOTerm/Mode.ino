State initial_state(char c) {
  switch (c)
  {
    // Single-character functions
    case '\x7': // BEL (bell)
      // FIXME: Blink the whole screen or something.
      return (State) &initial_state;
    case '\x0d': // CR  (carriage return)
      current_cursor.x = 1;
      return (State) &initial_state;
    case '\x5': // ENQ (enquiry)
      Serial.print("TTyGO v.0.0.1");
      return (State) &initial_state;
    case '\x9': // TAB (horizontal tab)
      terminal_cursor_move_to_tab(1);
      return (State) &initial_state;

    // MISSING: case '\x0e': // SO  (shift out)
    // MISSING: case '\x0f': // SI  (shift in)

      return (State) &initial_state;
    case ESC:
      if (debug_parsing) Serial.print("initial.ESC\n");
      newline_eating_mode = 0;
      return (State) &command_mode;

    // case '\x20': // SPACE
    // case '\x0c': // FF  (NP form feed, new page)
    // case '\x0a': // LF  (NL line feed, new line)
    // case '\x0b': // VT  (vertical tab)
    // case '\b': // BS  (backspace)
    // case '\n': // LF  (newline)
    default:
      terminal_put(c);
      return (State) &initial_state;
  }
}
