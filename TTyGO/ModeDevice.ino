char dcs_prev_char;
int dcs_btn;
int dcs_charpos;

State device_control(char c)
{
  if (debug_parsing) { Serial.print(S("DCS.start\n")); Serial.flush(); }
  dcs_prev_char = NULL;
  dcs_btn = 0;
  dcs_charpos = 0;
  return device_control_dummy_entry_1(c);
}

State device_control_dummy_entry_1(char c)
{
  if (debug_parsing) { Serial.print(S("DCS.dummy1\n")); Serial.flush(); }
  switch (c)
  {
    case '\x9c':
      return (State) &initial_state;
    case ';':
      return (State) &device_control_dummy_entry_2;
    default:
      return (State) &device_control_dummy_entry_1;
  }
}


State device_control_dummy_entry_2(char c)
{
  if (debug_parsing) { Serial.print(S("DCS.dummy2\n")); Serial.flush(); }
  switch (c)
  {
    case '\x9c':
      return (State) &initial_state;
    case ';':
      dcs_btn = 0;
      return (State) &device_control_entry_btn;
    default:
      return (State) &device_control_dummy_entry_2;
  }
}

State device_control_entry_btn(char c)
{
  if (debug_parsing) { Serial.print(S("DCS.btn\n")); Serial.flush(); }
  switch (c)
  {
    case '\x9c':
      return (State) &initial_state;

    case '/':
      dcs_prev_char = NULL;
      dcs_charpos = 0;
      buttons[dcs_btn][0] = NUL;
      return (State) &device_control_entry_str;

    default:
      dcs_btn = dcs_btn * 10 + (c - '0'); // Parse key index
      dcs_btn = dcs_btn % buttons_nr;
      return (State) &device_control_entry_btn;
  }
}

char hexval(char c) {
  if (c - '0' < 10) return c - '0';
  if (c - 'a' < 6) return c - 'a' + 10;
  if (c - 'A' < 6) return c - 'A' + 10;
  return 0; // ERROR ERROR ON THE WALL :P
}

State device_control_entry_str(char c)
{
  if (debug_parsing) { Serial.print(S("DCS.str\n")); Serial.flush(); }
  switch (c)
  {
    case '\x9c':
      if (debug_parsing)
      {
        Serial.print(S("Set button ")); Serial.print(dcs_btn); Serial.print(S(" to >"));
        Serial.print(buttons[dcs_btn]); Serial.print(S("<\n")); Serial.flush();
      }
      return (State) &initial_state;

    case ';':
      if (debug_parsing)
      {
        Serial.print(S("Set button ")); Serial.print(dcs_btn); Serial.print(S(" to >"));
        Serial.print(buttons[dcs_btn]); Serial.print(S("<\n")); Serial.flush();
      }
      dcs_btn = 0;
      return (State) &device_control_entry_btn;

    default:
      if (dcs_prev_char == NUL)
      {
        dcs_prev_char = c;
      }
      else
      {
        if (dcs_charpos < BUTTON_STRLEN) {
          buttons[dcs_btn][dcs_charpos] = (hexval(dcs_prev_char) << 4) + hexval(c);
          dcs_charpos++;
          buttons[dcs_btn][dcs_charpos] = NUL;
        }
        dcs_prev_char = NUL;
      }
      return (State) &device_control_entry_str;
  }
}
