void button_left_click()
{
  x--;
}

void button_right_click()
{
  x++;
}

void button_down_click()
{
  y++;
}

void button_up_click()
{
  y--;
}

void button_a_click()
{
  u8g2.sendF("c", 0x0a7); // Invert display
}

void button_b_click()
{
  u8g2.sendF("c", 0x0a6); // Restore display
}

void attach_buttons()
{
  // Set buttons (all supported modes: https://github.com/mathertel/OneButton/blob/master/examples/TwoButtons/TwoButtons.ino )
  button_left.attachClick(button_left_click);
  button_right.attachClick(button_right_click);
  button_down.attachClick(button_down_click);
  button_up.attachClick(button_up_click);
  button_a.attachClick(button_a_click);
  button_b.attachClick(button_b_click);
}
