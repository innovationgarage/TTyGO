State initial_state(char c) {
  switch (c)
  {
  case ESC:
    if (debug_parsing) Serial.print("initial.ESC\n");
    newline_eating_mode = 0;
    return (State) &command_mode;
  default:
//    if (debug_parsing) { Serial.print("initial.CHAR"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
    terminal_put(c);
    return (State) &initial_state;
  }
}

State command_mode(char c) {
  switch (c)
  {
  case '7':
    terminal_cursor_save();
    return (State) &initial_state;
  case '8':
    terminal_cursor_restore();
    return (State) &initial_state;
  case 'F':
    current_cursor.x = 1;
    current_cursor.y = terminal_height;
    return (State) &initial_state;
  case 'c': // Reset to initial state
    if (debug_parsing) { Serial.print("command.RESET\n"); Serial.flush(); }
    terminal_clear();
    return (State) &initial_state;
  case CSI:
    if (debug_parsing) { Serial.print("command.CSI\n"); Serial.flush(); }
    return (State) &control_sequence;
  default:
    if (debug_parsing) { Serial.print("command.ERROR"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
    return (State) &initial_state;
  }
}

