/* Hardware mapping and software quirks for the Arduboy production model platform */


// ************************************************
// **** Hardware mapping configuration  ***********
// ************************************************

// Hardware button mappings. Arguments are the same as for the
// OneButton() constructor.
// Each of these defines two programmable function keys: One for short
// click, one for long click.
// Up, A, Down, Left, Right, B
#define BUTTONS BUTTON(A0, true) BUTTON(7, true) BUTTON(A3, true) BUTTON(A2, true) BUTTON(A1, true) BUTTON(8, true)

#define BTN_DEBOUNCE_SPEED 40 // Debouncer: default 50 ms
#define BTN_CLICK_SPEED 300 // Click: default 600 ms
#define BTN_PRESS_SPEED 500 // Long press: default 1000 ms

// Display and display controller
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name
// for names.
#define DISPLAY_CONTROLLER SSD1306
#define DISPLAY_NAME 128X64_NONAME
#define DISPLAY_COMM 4W_HW_SPI
// Constructor arguments for the display controller
#define DISPLAY_ARGS U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6

#define LCD_CLK 5
#define LCD_SDA 4

#define LCD_FPS_TARGET 60

#define DEFAULT_SERIAL_BAUDS 9600

// ************************************************
// **** Software feature configuration  ***********
// ************************************************

#define DEFAULT_WIDECHAR 1
#define DEFAULT_TERMINAL_MAX_WIDTH 32
#define DEFAULT_TERMINAL_MAX_HEIGHT 12
#define DEFAULT_BUTTON_STRLEN 5
#define DEFAULT_DEC_SPECIAL_CHARACTER_SET 0
#define DEFAULT_DISPLAY_BUFFER F
#define DEFAULT_SERIAL_BUFFER_SIZE 0
