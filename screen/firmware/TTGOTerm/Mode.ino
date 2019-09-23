State initial_state(char c) {
  switch (c)
  {
  case ESC:
    if (debug_parsing) Serial.print("initial.ESC\n");
    return (State) &command_mode;
  default:
    if (debug_parsing) { Serial.print("initial.CHAR"); Serial.print((int)c); Serial.print("\n"); }
    terminal_put(c);
    return (State) &initial_state;
  }
}

State command_mode(char c) {
  switch (c)
  {
  case 'c': // Reset to initial state
    if (debug_parsing) Serial.print("command.RESET\n");
    terminal_clear();
    return (State) &initial_state;
  case CSI:
    if (debug_parsing) Serial.print("command.CSI\n");
    return (State) &control_sequence;
  default:
    if (debug_parsing) { Serial.print("command.ERROR"); Serial.print((int)c); Serial.print("\n"); }
    return (State) &initial_state;
  }
}

