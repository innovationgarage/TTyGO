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
