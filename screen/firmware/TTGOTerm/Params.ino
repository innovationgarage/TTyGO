// Parses a numeric parameter from the param buffer
void param_temp_buffer_digest(int default_value)
{
  if (param_temp_buffer_pos > 0)
    control_sequence_param[control_sequence_param_pos] = atoi(param_temp_buffer); // What happens in case of error?
  else
    control_sequence_param[control_sequence_param_pos] = default_value;

  control_sequence_param_pos++;
  param_temp_buffer_pos = 0;
}

// Feeds a char to the param buffer
void param_temp_buffer_eat(char c)
{
  param_temp_buffer[param_temp_buffer_pos++] = c;
  param_temp_buffer[param_temp_buffer_pos] = NUL;
}

