State initial_state(char c) {
  char *conversion_str;

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
      Serial.print(S("TTyGO v.0.0.1"));
      return (State) &initial_state;
    case '\x9': // TAB (horizontal tab)
      terminal_cursor_move_to_tab(1);
      return (State) &initial_state;

    case '\x0e': // SO  (shift out)
      current_charset = 1;
      return (State) &initial_state;
    case '\x0f': // SI  (shift in)
      current_charset = 0;
      return (State) &initial_state;
      
    case ESC:
      if (debug_parsing) { Serial.print(S("initial.ESC\n")); Serial.flush(); }
      newline_eating_mode = 0;
      return (State) &command_mode;

    case '\x0c': // FF  (NP form feed, new page)
    case '\x0b': // VT  (vertical tab)
    case '\n': // LF  (newline)
      terminal_newline();
      return (State) &initial_state;
    
    case '\b': // BS  (backspace)
      terminal_backspace();
      return (State) &initial_state;

    default:
      switch (charsets[current_charset])
      {
        case '0':
          for(conversion_str = dec_special_character_set(c); conversion_str && *conversion_str; conversion_str++) {
            parse_utf_8_sequence(*conversion_str);
          }
          if (!conversion_str) {
            parse_utf_8_sequence(c);
          }
          return (State) &initial_state;

        // case '@': // Bzzt WRONG! For this we should do ISO 8859-1 translation, but we won't. Mwahahaha
        // case 'G':
        // case 'B':
        default:
          return parse_utf_8_sequence(c);
      }
  }
}
