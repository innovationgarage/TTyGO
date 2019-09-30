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
      Serial.print("TTyGO v.0.0.1");
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
      if (debug_parsing) Serial.print("initial.ESC\n");
      newline_eating_mode = 0;
      return (State) &command_mode;

    case '\x0c': // FF  (NP form feed, new page)
    case '\x0b': // VT  (vertical tab)
    case '\b': // BS  (backspace)
    case '\n': // LF  (newline)
      terminal_put(c);
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

#ifdef WIDECHAR
int glyphpos = 0;
Glyph current_glyph = {};
State parse_utf_8_sequence(char c) {
  // Serial.print("UTF-8 "); Serial.print((int) c); Serial.print("@"); Serial.print(glyphpos); Serial.print(": "); Serial.print(" ");
  if (!(c & 0b10000000)) { // Not continuation nor start of multibyte-glyph
    if (glyphpos == 0) {
      //Serial.print("SINGLECHAR NEW >"); serial_print_glyph({c}); Serial.print("<\n");
      terminal_put_glyph({c});
      return (State) &initial_state;
    } else {
      //Serial.print("SINGLECHAR GLYPH >"); serial_print_glyph(current_glyph); Serial.print("<\n");
      terminal_put_glyph(current_glyph);
      glyphpos = 0;
      current_glyph = {};
      return initial_state(c);
    }
  }
  if (glyphpos > 3 || (((c & 0b11000000) == 0b11000000) && (glyphpos != 0))) { // Start of new multibyte-glyph
    //Serial.print("NEWMULTI GLYPH >"); serial_print_glyph(current_glyph); Serial.print("<\n");
    terminal_put_glyph(current_glyph);
    glyphpos = 0;
    current_glyph = {};
    return initial_state(c);
  }

  // Serial.print("CONT\n");

  switch (glyphpos)
  {
    case 0: current_glyph.a = c; break;
    case 1: current_glyph.b = c; break;
    case 2: current_glyph.c = c; break;
    case 3: current_glyph.d = c; break;
  }
  glyphpos++;

  return (State) &parse_utf_8_sequence;
}
#else
State parse_utf_8_sequence(char c) {
  terminal_put_glyph({c});
  return (State) &initial_state;
}
#endif