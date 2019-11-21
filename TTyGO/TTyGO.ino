#define VERSION "0.0.2"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#include <ESP8266WiFi.h>
#include "config.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "OneButton.h"

#define CEIL_DIV(dividend, divisor) (1 + ((dividend - 1) / divisor))

#ifdef USE_ESP8266SCHEDULER
  #include <Scheduler.h> // ESP8266Scheduler
#else
  #include "scheduler.h"
#endif

#ifdef FLASH_STRINGS
  #define S(s) F(s)
  #define S_len(s) strlen_P(s)
  #define S_get(s, i) pgm_read_byte(s + i)
#else
  #define S(s) s
  #define S_len(s) strlen(s)
  #define S_get(s, i) (s[i])
#endif

#define VARIABLE 3
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define NAME(fun) EVALUATOR(fun, VARIABLE)


#define DISPLAY_CLASS2(DISPLAY_CONTROLLER, DISPLAY_NAME, DISPLAY_BUFFER, DISPLAY_COMM) U8G2_ ## DISPLAY_CONTROLLER ## _ ## DISPLAY_NAME ## _ ## DISPLAY_BUFFER ## _ ## DISPLAY_COMM
#define DISPLAY_CLASS(DISPLAY_CONTROLLER, DISPLAY_NAME, DISPLAY_BUFFER, DISPLAY_COMM) DISPLAY_CLASS2(DISPLAY_CONTROLLER, DISPLAY_NAME, DISPLAY_BUFFER, DISPLAY_COMM) 
DISPLAY_CLASS(DISPLAY_CONTROLLER, DISPLAY_NAME, DISPLAY_BUFFER, DISPLAY_COMM) u8g2(DISPLAY_ARGS);

// ASCII control characters recognised
const char CR = '\r', LF = '\n', VT = '\v', BS = '\b', FF = '\f', ESC = '\e', NUL = '\0', CSI = '[';

#define BIT(arr, idx) ((arr[(idx) / 8] & (1 << ((idx) % 8))) >> ((idx) % 8))
#define BIT_SET(arr, idx, val) (arr[(idx) / 8] = ((arr[(idx) / 8] & ~(1 << ((idx) % 8))) | ((val) << ((idx) % 8))))

#define TERMINAL_COUNT TERMINAL_MAX_WIDTH * TERMINAL_MAX_HEIGHT
#define TERMINAL_DIRTY_COUNT CEIL_DIV(TERMINAL_MAX_WIDTH  * TERMINAL_MAX_HEIGHT, 8)
#define TERMINAL_TABS_COUNT CEIL_DIV(TERMINAL_MAX_WIDTH, 8)

#define TERM(x, y) (terminal_buffer[((x) - 1) + ((y) - 1)*terminal_width])
#define TERM_SET(x, y, value) do { TERM(x, y) = value; TERM_DIRTY_SET(x, y, 1); } while(0)
#define TERM_DIRTY(x, y) BIT(terminal_buffer_dirty, ((x) - 1) + ((y) - 1)*terminal_width)
#define TERM_DIRTY_SET(x, y, val) BIT_SET(terminal_buffer_dirty, ((x) - 1) + ((y) - 1)*terminal_width, val)

typedef struct {
  int x;
  int y;
} Cursor;

typedef struct {
  int upper;
  int lower;
} ScrollRegion;

typedef struct {
  char a;
  #if WIDECHAR > 1
  char b;
  #if WIDECHAR > 2
  char c;
  #if WIDECHAR > 3
  char d;
  #endif
  #endif
  #endif
} Glyph;

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
void terminal_backspace();
void terminal_newline();
void terminal_put_glyph(Glyph g);
void terminal_reset();

void param_temp_buffer_digest(int default_value = 1);
void param_temp_buffer_eat(char c);

extern char charsets[4];
extern char current_charset;
// This is all set by the terminal_setup based on current font and display size
extern ScrollRegion scroll_region;
extern Cursor current_cursor, saved_cursor;
extern unsigned char terminal_tab_stops[TERMINAL_TABS_COUNT];
extern int char_height, char_width,
       terminal_width, terminal_height, display_height_offset, display_width_offset;
extern Glyph terminal_buffer[TERMINAL_COUNT];
extern unsigned char terminal_buffer_dirty[TERMINAL_DIRTY_COUNT];

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
State parse_utf_8_sequence(char c);
State command_mode(char c);
State designate_g0(char c);
State designate_g1(char c);
State designate_g2(char c);
State designate_g3(char c);
State control_sequence(char c);
State control_sequence_entry(char c);
State device_control(char c);
State device_control_dummy_entry_1(char c);
State device_control_dummy_entry_2(char c);
State device_control_entry_btn(char c);
State device_control_entry_str(char c);


char *dec_special_character_set(unsigned char c);

void serial_print_glyph(Glyph g);

void reset_buttons();
void reset_button(int i);
extern OneButton phys_buttons[];
extern const size_t phys_buttons_nr;
extern const size_t buttons_nr;
extern char buttons[][BUTTON_STRLEN];

void serial_print_glyph(Glyph g) {
  Serial.print(g.a);
  #if WIDECHAR > 1
  if (g.b != NUL) Serial.print(g.b);
  #if WIDECHAR > 2
  if (g.c != NUL) Serial.print(g.c);
  #if WIDECHAR > 3
  if (g.d != NUL) Serial.print(g.d);
  #endif
  #endif
  #endif
}

void buffer_serial();
char serial_buffer_data_available();
int serial_buffer_get();
