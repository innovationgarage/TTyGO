State command_mode(char c) {
  switch (c)
  {
    // MISSING: ESC D Index ( IND is 0x84)
    // MISSING: ESC E Next Line ( NEL is 0x85)
    
    case 'H': // ESC H Tab Set ( HTS is 0x88)
      BIT_SET(terminal_tab_stops, current_cursor.x, 1);
      return (State) &control_sequence;

    // MISSING: ESC M Reverse Index ( RI is 0x8d)
    // MISSING: ESC N Single Shift Select of G2 Character Set ( SS2 is 0x8e): affects next character only
    // MISSING: ESC O Single Shift Select of G3 Character Set ( SS3 is 0x8f): affects next character only
    case 'P': // ESC P Device Control String ( DCS is 0x90)
      return (State) &device_control;

    // MISSING: ESC V Start of Guarded Area ( SPA is 0x96)
    // MISSING: ESC W End of Guarded Area ( EPA is 0x97)
    // MISSING: ESC X Start of String ( SOS is 0x98)
    // MISSING: ESC Z Return Terminal ID (DECID is 0x9a). Obsolete form of CSI c (DA).
    
    case CSI: // ESC [ Control Sequence Introducer ( CSI is 0x9b)
      if (debug_parsing) { Serial.print(S("command.CSI\n")); Serial.flush(); }
      return (State) &control_sequence;
    
    // MISSING: ESC \ String Terminator ( ST is 0x9c)
    // MISSING: ESC ] Operating System Command ( OSC is 0x9d)
    // MISSING: ESC ^ Privacy Message ( PM is 0x9e)
    // MISSING: ESC _ Application Program Command ( APC is 0x9f)
    // MISSING: ESC SP F 7-bit controls (S7C1T).
    // MISSING: ESC SP G 8-bit controls (S8C1T).
    // MISSING: ESC SP L Set ANSI conformance level 1 (dpANS X3.134.1).
    // MISSING: ESC SP M Set ANSI conformance level 2 (dpANS X3.134.1).
    // MISSING: ESC SP N Set ANSI conformance level 3 (dpANS X3.134.1).
    // MISSING: ESC # 3 DEC double-height line, top half (DECDHL)
    // MISSING: ESC # 4 DEC double-height line, bottom half (DECDHL)
    // MISSING: ESC # 5 DEC single-width line (DECSWL)
    // MISSING: ESC # 6 DEC double-width line (DECDWL)
    // MISSING: ESC # 8 DEC Screen Alignment Test (DECALN)
    
    case '%': // ESC % @ Select default character set, ISO 8859-1 (ISO 2022)
              // ESC % G Select UTF-8 character set (ISO 2022)
      return (State) &designate_g0;
    case '(': // ESC ( Ch Designate G0 Character Set (ISO 2022)
      return (State) &designate_g0;
    case ')': // ESC ) Ch Designate G1 Character Set (ISO 2022)
      return (State) &designate_g1;
    case '*': // ESC * Ch Designate G2 Character Set (ISO 2022)
      return (State) &designate_g2;
    case '+': // ESC + Ch Designate G3 Character Set (ISO 2022)
      return (State) &designate_g3;

    case '7':// ESC 7 Save Cursor (DECSC)
      terminal_cursor_save();
      return (State) &initial_state;
    case '8': // ESC 8 Restore Cursor (DECRC)
      terminal_cursor_restore();
      return (State) &initial_state;
    
    // MISSING: ESC = Application Keypad (DECPAM)
    // MISSING: ESC > Normal Keypad (DECPNM)
    
    case 'F': // ESC F Cursor to lower left corner of screen
      current_cursor.x = 1;
      current_cursor.y = terminal_height;
      return (State) &initial_state;
    
    case 'c': // ESC c Full Reset (RIS)
      if (debug_parsing) { Serial.print(S("command.RESET\n")); Serial.flush(); }
      reset_buttons();
      terminal_reset();
      terminal_clear();
      return (State) &initial_state;

    // MISSING: ESC l Memory Lock (per HP terminals). Locks memory above the cursor.
    // MISSING: ESC m Memory Unlock (per HP terminals)
    // MISSING: ESC n Invoke the G2 Character Set as GL (LS2).
    // MISSING: ESC o Invoke the G3 Character Set as GL (LS3).
    // MISSING: ESC | Invoke the G3 Character Set as GR (LS3R).
    // MISSING: ESC } Invoke the G2 Character Set as GR (LS2R).
    // MISSING: ESC ~ Invoke the G1 Character Set as GR (LS1R).
    
    default:
      if (debug_parsing) { Serial.print(S("command.ERROR")); Serial.print((int)c); Serial.print(S("\n")); Serial.flush(); }
      return (State) &initial_state;
  }
}

State designate_g0(char c) {
  charsets[0] = c;
  return (State) &initial_state;
}
State designate_g1(char c) {
  charsets[1] = c;
  return (State) &initial_state;
}
State designate_g2(char c) {
  charsets[2] = c;
  return (State) &initial_state;
}
State designate_g3(char c) {
  charsets[3] = c;
  return (State) &initial_state;
}
