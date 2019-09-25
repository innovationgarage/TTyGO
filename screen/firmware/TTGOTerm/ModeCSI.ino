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


    // These appear in order or final character, as they do in https://www.xfree86.org/current/ctlseqs.html
    // Unimplemented CSI codes from that document have a MISSING comment here

    case '@': // CSI Ps @ Insert Blank Character(s)
      param_temp_buffer_digest(1);

      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_put(' ');
      }
      return (State) &initial_state;

    case 'A': // CSI Ps A Cursor Up
    case 'B': // CSI Ps B Cursor Down
    case 'C': // CSI Ps C Cursor Forward
    case 'D': // CSI Ps D Cursor Backward
      param_temp_buffer_digest();
      current_cursor.x += ((c == 'C' || c == 'D') ? ((c == 'D' ? -1 : 1) * control_sequence_param[0]) : 0);
      current_cursor.y += ((c == 'A' || c == 'B') ? ((c == 'A' ? -1 : 1) * control_sequence_param[0]) : 0);
      return (State) &initial_state;

    case 'E': // CSI Ps E Cursor Next Line
    case 'F': // CSI Ps F Cursor Preceding Line
      param_temp_buffer_digest();

      if (debug_parsing) { Serial.print("CSI.EF"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      current_cursor.x = 1;
      current_cursor.y += (c == 'E' ? 1 : -1) * control_sequence_param[0];
      return (State) &initial_state;

    case 'G': // CSI Ps G Cursor Character Absolute
      param_temp_buffer_digest();
      if (debug_parsing) { Serial.print("CSI.character_absolute"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      current_cursor.x = control_sequence_param[0];
      return (State) &initial_state;

    case 'H': // CSI Ps ; Ps H Cursor Position
      param_temp_buffer_digest();
      if (debug_parsing) { Serial.print("CSI.H\n"); Serial.flush(); }
      current_cursor.y =  control_sequence_param[0];
      current_cursor.x = control_sequence_param_pos >= 1 ? control_sequence_param[1] : 1;
      return (State) &initial_state;

    case 'I': // CSI Ps I Cursor Forward Tabulation
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_cursor_move_to_tab(1);
      }
      return (State) &initial_state;
    
    case 'J': // CSI Ps J Erase in Display
      if (debug_parsing) Serial.print("CSI.J\n");
      param_temp_buffer_digest(0);
      terminal_clear(control_sequence_param[0]);
      return (State) &initial_state;

    // MISSING CSI ? Ps J Erase in Display

    case 'K': // CSI Ps K Erase in Line
      param_temp_buffer_digest(0);
      terminal_clear_line(current_cursor.x, current_cursor.y, control_sequence_param[0]);
      return (State) &initial_state;

    // MISSING: CSI ? Ps K Erase in Line

    case 'L': // CSI Ps L Insert Lines
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(current_cursor.y, terminal_height, 0);
        terminal_clear_line(0, current_cursor.y, 2);
      }
      return (State) &initial_state;
    
    case 'M': // CSI Ps M Delete Lines
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(current_cursor.y, terminal_height, 1);
        terminal_clear_line(0, scroll_region.lower, 2);
      }
      return (State) &initial_state;

    case 'P': // CSI Ps P Delete Characters
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll_line(current_cursor.y, current_cursor.x, terminal_width, 1);
        TERM(terminal_width, current_cursor.y) = ' ';
      }
      return (State) &initial_state;

    case 'S': // CSI Ps S Scroll up
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(0, terminal_height, 1);
        terminal_clear_line(0, scroll_region.lower, 2);
      }
      return (State) &initial_state;

    case 'T': // CSI Ps T Scroll down
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(0, terminal_height, 0);
        terminal_clear_line(0, scroll_region.upper, 2);
      }
      return (State) &initial_state;

    // MISSING: CSI Ps ; Ps ; Ps ; Ps ; Ps T Initiate highlight mouse tracking

    case 'X': // CSI Ps X Erase Characters
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        TERM(current_cursor.x + i, current_cursor.y) = ' ';
      }
      return (State) &initial_state;

    case 'Z': // CSI Ps Z Cursor Backward Tabulation
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_cursor_move_to_tab(0);
      }
      return (State) &initial_state;

    case '`': // CSI P m ` Character Position Absolute
      // MISSING: CSI Pm `
      return (State) &initial_state;

    case 'b': // CSI Ps b Repeat the preceding character
      param_temp_buffer_digest(1);
      terminal_scroll_line(current_cursor.y, current_cursor.x-1, current_cursor.x+control_sequence_param[0]-1, 0);
      return (State) &initial_state;

    case 'c': // CSI Ps c Send Device Attributes
      // Tell the user we're a VT220 with a printer (our wifi?) and no extra features...
      Serial.print("\x1b[?60;2;c");
      Serial.flush();

      return (State) &initial_state;

    // MISSING: CSI > Ps c Send Device Attributes

    case 'd': // CSI P m d Line Position Absolute
      param_temp_buffer_digest(1);
      current_cursor.y = control_sequence_param[0];
      return (State) &initial_state;

    case 'f': // CSI Ps ; Ps f Horizontal and Vertical Position
      param_temp_buffer_digest(1);
      current_cursor.y = control_sequence_param[0];
      current_cursor.x = control_sequence_param[1];
      return (State) &initial_state;

    // MISSING: CSI P s g Tab Clear (TBC)
    // MISSING: CSI P m h Set Mode (SM)
    // MISSING: CSI ? P m h DEC Private Mode Set (DECSET)
    
    case 'i': // CSI P m i Media Copy (MC)
      param_temp_buffer_digest(0);

      
      switch (control_sequence_param[0])
      {
        case 0:
          Serial.print("\x1b[200~");
          for (int y = 1; y <= terminal_height; y++) {
            for (int x = 1; x <= terminal_width; x++) {
              if (TERM(x, y) != NUL) {
                Serial.print(TERM(x, y));
              }
            }
            Serial.print("\n");
            Serial.flush();
          }
          Serial.print("\x1b[201~");
          Serial.flush();
      }
      return (State) &initial_state;
    
    // MISSING: CSI ? P m i Media Copy (MC, DEC-specific)
    // MISSING: CSI P m l Reset Mode (RM)
    // MISSING: CSI ? P m l DEC Private Mode Reset (DECRST)
    // MISSING: CSI P m m Character Attributes (SGR)
    // MISSING: CSI P s n Device Status Report (DSR)
    // MISSING: CSI ? P s n Device Status Report (DSR, DEC-specific)
    // MISSING: CSI ! p Soft terminal reset (DECSTR)
    // MISSING: CSI P s ; P s “ p Set conformance level (DECSCL) Valid values for the first parameter:
    // MISSING: CSI P s “ q Select character protection attribute (DECSCA).
    // MISSING: CSI P s ; P s r Set Scrolling Region [top;bottom]
    // MISSING: CSI ? P m r Restore DEC Private Mode Values.
    // MISSING: CSI P t ; P l ; P b ; P r ; P s $ r Change Attributes in Rectangular Area (DECCARA).
    // MISSING: CSI s Save cursor (ANSI.SYS)
    // MISSING: CSI ? P m s Save DEC Private Mode Values.
    
    case 't': // CSI P s ; P s ; P s t Window manipulation - Other control seqs https://www.xfree86.org/current/ctlseqs.html
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

    
    // MISSING: CSI P t ; P l ; P b ; P r ; P s $ t Reverse Attributes in Rectangular Area (DECRARA).
    // MISSING: CSI u Save cursor (ANSI.SYS)
    // MISSING: CSI P t ; P l ; P b ; P r ; P p ; P t ; P l ; P p $ v Copy Rectangular Area (DECCRA)
    // MISSING: CSI P t ; P l ; P b ; P r ’ w Enable Filter Rectangle (DECEFR)
    // MISSING: CSI P s x Request Terminal Parameters (DECREQTPARM)
    // MISSING: CSI P s x Select Attribute Change Extent (DECSACE).
    // MISSING: CSI P c ; P t ; P l ; P b ; P r $ x Fill Rectangular Area (DECFRA).
    // MISSING: CSI P s ; P u ’ z Enable Locator Reporting (DECELR)
    // MISSING: CSI P t ; P l ; P b ; P r $ z Erase Rectangular Area (DECERA).
    // MISSING: CSI P m ’ { Select Locator Events (DECSLE)
    // MISSING: CSI P t ; P l ; P b ; P r $ { Selective Erase Rectangular Area (DECSERA).
    // MISSING: CSI P s ’ | Request Locator Position (DECRQLP)

    default:
      return (State) &initial_state;
  }
}

