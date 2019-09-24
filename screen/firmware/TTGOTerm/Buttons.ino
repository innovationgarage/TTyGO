void button_send_key(char c)
{
  Serial.print(ESC);
  Serial.print(CSI);
  Serial.print(c);
}

// Cursor up
void button_left_click()
{
  button_send_key('A');
}

// Enter
void button_middle_click()
{
  Serial.print(CR);
}

// Cursor down
void button_right_click()
{
  button_send_key('B');
}

void buttons_setup()
{
  // Set buttons (all supported modes: https://github.com/TanPitch/ButtonKing/blob/master/examples/ButtonQueen/ButtonQueen.ino )
  button_left.setClick(button_left_click);
  button_middle.setClick(button_middle_click);
  button_right.setClick(button_right_click);
}