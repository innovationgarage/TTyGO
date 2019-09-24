State control_sequence(char c) {
  if (debug_parsing) { Serial.print("CSI.start\n"); Serial.flush(); }
  control_sequence_param_pos = 0;
  param_temp_buffer_pos = 0;
  return control_sequence_entry(c);
}

State control_sequence_entry(char c) {
  switch (c)
  {
    case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
      if (debug_parsing) { Serial.print("CSI.param");  Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      param_temp_buffer_eat(c);
      return (State) &control_sequence_entry;

    case ';':
      if (debug_parsing) { Serial.print("CSI.newparam"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      param_temp_buffer_digest(); // Prepares for next parameter... or control code
      return (State) &control_sequence_entry;

    case 'P': // Other control seqs https://www.xfree86.org/current/ctlseqs.html
      if (debug_parsing) Serial.print("CSI.P\n");
      param_temp_buffer_digest();
      switch (control_sequence_param[0])
      {
        case 1:
          switch (control_sequence_param[1])
          {
            case 8: // Report the size of the text area in characters as CSI 8 ; height ; width t
            case 9: // Report the size of the screen in characters as CSI 9 ; height ; width t
              Serial.print("[");
              Serial.print(terminal_height);
              Serial.print(";");
              Serial.print(terminal_width);
              Serial.print("t");
              Serial.flush();
          }
      }
      return (State) &initial_state;

    case 'J': // Clear screen
      if (debug_parsing) Serial.print("CSI.J\n");
      param_temp_buffer_digest(0);
      terminal_clear(control_sequence_param[0]);
      return (State) &initial_state;

    default: // For A,B,C,D,E,F,G,H
      param_temp_buffer_digest();

      // Absolute cursor pos
      if (c == 'H')
      {
        if (debug_parsing) { Serial.print("CSI.H\n"); Serial.flush(); }
        cursor_top =  control_sequence_param[0];
        cursor_left = control_sequence_param_pos >= 1 ? control_sequence_param[1] : 1;
      }
      else
      {
        // Line based (E,F)
        if (c == 'E' || c == 'F')
        {
          if (debug_parsing) { Serial.print("CSI.EF"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
          cursor_left = 1;
          cursor_top += (c == 'E' ? 1 : -1) * control_sequence_param[0];
        }
        else
        {
        if (debug_parsing) { Serial.print("CSI.relative"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
          cursor_left = (c == 'G') ? control_sequence_param[0] : cursor_left;

          // Relative
          cursor_left += ((c == 'C' || c == 'D') ? ((c == 'D' ? -1 : 1) * control_sequence_param[0]) : 0);
          cursor_top += ((c == 'A' || c == 'B') ? ((c == 'A' ? -1 : 1) * control_sequence_param[0]) : 0);
        }
      }
      return (State) &initial_state;
  }
}

