#include <U8g2lib.h>
#include <Wire.h>
#include "OneButton.h"
#include <Scheduler.h> // ESP8266Scheduler

#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

#define LCD_CLK 5
#define LCD_SDA 4
#define LCD_FPS_TARGET 60

#define SERIAL_BAUDS 9600

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall
#define lcd_font u8g2_font_4x6_mf

// ASCII control characters recognised
const char CR = '\r', LF = '\n', VT = '\v', BS = '\b', FF = '\f', ESC = '\e', NUL = '\0', CSI = '[';

#define TERM(x, y) (terminal_buffer[(x - 1) + (y - 1)*terminal_width])

#define BIT(arr, idx) ((arr[idx / 8] & (1 << (idx % 8))) >> (idx % 8))
#define BIT_SET(arr, idx, val) (arr[idx / 8] = ((arr[idx / 8] & ~(1 << (idx % 8))) | (val << (idx % 8))))

typedef struct {
  int x;
  int y;
} Cursor;

typedef struct {
  int upper;
  int lower;
} ScrollRegion;

void terminal_cursor_move_to_tab(int next=1);

void terminal_scroll(int start, int end, int up);
void terminal_scroll_line(int y, int start, int end, int direction_left = 0);
void terminal_clear_line(int x, int y, int mode =  2);

void terminal_cursor_save();
void terminal_cursor_restore();
void terminal_setcursor(int col, int row);
void terminal_draw();
void terminal_clear(int mode = 2);
void terminal_setup();
void terminal_put(char c);
void param_temp_buffer_digest(int default_value = 1);
void param_temp_buffer_eat(char c);

// This is all set by the terminal_setup based on current font and display size
extern ScrollRegion scroll_region;
extern Cursor current_cursor, saved_cursor;
extern unsigned char terminal_tab_stops[80/8];
extern int char_height, char_width,
       terminal_width, terminal_height, display_height_offset, display_width_offset;
// Just a maximum, scrolling is not implemented
extern char terminal_buffer[80 * 80];

extern int newline_eating_mode;

// Control sequences and internal buffers
const int param_temp_buffer_max = 8;
char param_temp_buffer[param_temp_buffer_max];
int param_temp_buffer_pos = 0;

const int control_sequence_param_max = 3;
int control_sequence_param[control_sequence_param_max];
int control_sequence_param_pos = 0;


typedef void Function();
typedef Function *(*State)(char c);

const int debug_parsing = 0;

State initial_state(char c);
State command_mode(char c);
State control_sequence(char c);
State control_sequence_entry(char c);
