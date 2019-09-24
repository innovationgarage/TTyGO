#include <U8g2lib.h>
#include <Wire.h>
#include "ButtonKing.h"

#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

#define LCD_CLK 5
#define LCD_SDA 4

#define SERIAL_BAUDS 9600

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall 
#define lcd_font u8g2_font_4x6_mf

// ASCII control characters recognised
const char CR = '\r', LF = '\n', VT = '\v', BS = '\b', FF = '\f', ESC = '\e', NUL = '\0', CSI = '[';

ButtonKing button_left(BTN_LEFT, true), button_middle(BTN_MID, true), button_right(BTN_RIGHT, true);




void terminal_setcursor(int col, int row);
void terminal_draw();
void terminal_clear(int mode = 2);
void terminal_setup();
void terminal_put(char c);
void param_temp_buffer_digest(int default_value = 1);
void param_temp_buffer_eat(char c);

// This is all set by the terminal_setup based on current font and display size
extern int cursor_left, cursor_top, char_height, char_width,
    terminal_width, terminal_height, display_height_offset, display_width_offset;
// Just a maximum, scrolling is not implemented
extern char terminal_buffer[80 * 80];


// Control sequences and internal buffers
const int param_temp_buffer_max = 8;
char param_temp_buffer[param_temp_buffer_max];
int param_temp_buffer_pos = 0;

const int control_sequence_param_max = 3;
int control_sequence_param[control_sequence_param_max];
int control_sequence_param_pos = 0;


typedef void Function();
typedef Function *(*State)(char c);

int debug_parsing = 0;


State initial_state(char c);
State command_mode(char c);
State control_sequence(char c);
State control_sequence_entry(char c);

