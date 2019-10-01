#if WIDECHAR > 1
int glyphpos = 0;
Glyph current_glyph = {};
State parse_utf_8_sequence(char c) {
  if (debug_parsing)
  {
    Serial.print(S("UTF-8 ")); Serial.print((int) c); Serial.print(S("@"));
    Serial.print(glyphpos); Serial.print(S(": ")); Serial.print(S(" "));
  }
  if (!(c & 0b10000000)) { // Not continuation nor start of multibyte-glyph
    if (glyphpos == 0) {
      if (debug_parsing) { Serial.print(S("SINGLECHAR NEW >")); serial_print_glyph({c}); Serial.print(S("<\n")); Serial.flush(); }
      terminal_put_glyph({c});
      return (State) &initial_state;
    } else {
      if (debug_parsing) { Serial.print(S("SINGLECHAR GLYPH >")); serial_print_glyph(current_glyph); Serial.print(S("<\n")); Serial.flush(); }
      terminal_put_glyph(current_glyph);
      glyphpos = 0;
      current_glyph = {};
      return initial_state(c);
    }
  }
  if (glyphpos > 3 || (((c & 0b11000000) == 0b11000000) && (glyphpos != 0))) { // Start of new multibyte-glyph
    if (debug_parsing) { Serial.print(S("NEWMULTI GLYPH >")); serial_print_glyph(current_glyph); Serial.print(S("<\n")); Serial.flush(); }
    terminal_put_glyph(current_glyph);
    glyphpos = 0;
    current_glyph = {};
    return initial_state(c);
  }

  if (debug_parsing) { Serial.print(S("CONT\n")); Serial.flush(); }

  switch (glyphpos)
  {
    case 0: current_glyph.a = c; break;
    #if WIDECHAR > 1
    case 1: current_glyph.b = c; break;
    #if WIDECHAR > 2
    case 2: current_glyph.c = c; break;
    #if WIDECHAR > 3
    case 3: current_glyph.d = c; break;
    #endif
    #endif
    #endif
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
