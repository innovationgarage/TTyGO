/* Hardware mapping and software quirks for the TTGO ESP8266 platform */

// ************************************************
// **** Hardware mapping configuration  ***********
// ************************************************

// Hardware button mappings. Arguments are the same as for the
// OneButton() constructor.
// Each of these defines two programmable function keys: One for short
// click, one for long click.
// Left, Middle, Right
#define BUTTONS BUTTON(12, true) BUTTON(14, true) BUTTON(13, true)

#define BTN_DEBOUNCE_SPEED 40 // Debouncer: default 50 ms
#define BTN_CLICK_SPEED 300 // Click: default 600 ms
#define BTN_PRESS_SPEED 500 // Long press: default 1000 ms

// Display and display controller
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name
// for names.
#define DISPLAY_CONTROLLER SH1106 
#define DISPLAY_NAME 128X64_NONAME
#define DISPLAY_COMM HW_I2C
// Constructor arguments for the display controller
#define DISPLAY_ARGS U8G2_R0, /* reset=*/ U8X8_PIN_NONE

#define LCD_CLK 5
#define LCD_SDA 4
#define LCD_FPS_TARGET 30

// Use https://github.com/nrwiersma/ESP8266Scheduler for task scheduling
// Note: Only available on the ESP platform
// Comment the line below to use the "fake" scheduler
#define USE_ESP8266SCHEDULER

// ESP uses software I2C that needs configuring at startup
#define ESPI2C

#define DEFAULT_SERIAL_BAUDS 19200

// ************************************************
// **** Software feature configuration  ***********
// ************************************************

#define DEFAULT_WIDECHAR 2
#define DEFAULT_TERMINAL_MAX_WIDTH 40
#define DEFAULT_TERMINAL_MAX_HEIGHT 12
#define DEFAULT_BUTTON_STRLEN 10
#define DEFAULT_DEC_SPECIAL_CHARACTER_SET 1
#define DEFAULT_DISPLAY_BUFFER F
#define DEFAULT_SERIAL_BUFFER_SIZE 0
