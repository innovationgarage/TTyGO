/* Build configuration */


// ************************************************
// **** Hardware mapping configuration  ***********
// ************************************************
#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

#define BTN_DEBOUNCE_SPEED 40 // Debouncer: default 50 ms
#define BTN_CLICK_SPEED 300 // Click: default 600 ms
#define BTN_PRESS_SPEED 500 // Long press: default 1000 ms

#define LCD_CLK 5
#define LCD_SDA 4
#define LCD_FPS_TARGET 60

#define SERIAL_BAUDS 115200

// Use https://github.com/nrwiersma/ESP8266Scheduler for task scheduling
// Note: Only available on the ESP platform
// Comment the line below to use the "fake" scheduler
#define USE_ESP8266SCHEDULER

// Uncomment on the ESP platform: ESP uses software I2C that needs
// configuring at startup
#define ESPI2C

// ************************************************
// **** On Screen Keyboard configuration  *********
// ************************************************

// Check hardware mapping configuration for tunning the speed of they key repeats when holding and scrolling

// Comment out to disable the on screen keyboard
#define ON_SCREEN_KEYBOARD
#define OSK_KEYS_TO_SHOW_PER_SIDE 4 // How many keys appear to the sides of the currently selected key in the OSK
#define OSK_HOLD_DELAY 200 // Time for delaying the speedy scroll when holding a key: default 200
#define OSK_HOLD_DELAY_ACCELERATION 30 // Acceleration of the scroll when holding: default 30
#define OSK_OFFSET_KEYS_DEFAULT 3// Pixels from the left of the screen to the start of the first key to show
#define OSK_OFFSET_BOUNCE_LENGTH 7 // Graphical horizontal jump when scrolling
#define OSK_OFFSET_BOUNCE_SPEED 5 // Speed for returning to the center after the bounce jump

// ************************************************
// **** Software feature configuration  ***********
// ************************************************

// Store strings constants in flash rather than RAM
#define FLASH_STRINGS 1

// Speed for the blinking cursor
#define TERMINAL_CURSOR_BLINK_SPEED_LOOP 1000 // Total loop in ms
#define TERMINAL_CURSOR_BLINK_SPEED_ON 450 // Off time in ms

// Length of UTF-8 multi-byte sequences to support. If 1, only ASCII
// is supported. Max is 4.
// Note that font support for your codepoints is also required!
#define WIDECHAR 2

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall
#define LCD_FONT u8g2_font_4x6_mf

// Should be >= u8g2.getDisplayWidth() / font.char_width
#define TERMINAL_MAX_WIDTH 40
// Should be >= u8g2.getDisplayHeight() / font.char_height
#define TERMINAL_MAX_HEIGHT 24

// Max length of programmable button strings
#define BUTTON_STRLEN 10

// Banner printed when the device boots. Note that it is strongly
// recommended to start the banner by resetting the terminal using
// \x1bc\x1b[1;1H

#if WIDECHAR > 1
#define WIDECHARSTR " w UTF-8"
#define BANNER "Powered by räksmörgås\n"
#else
#define WIDECHARSTR ""
#define BANNER ""
#endif

#define TERMINAL_INIT_BANNER "\x1bc\x1b[1;1HTTyGO v." VERSION "\nVT220" WIDECHARSTR " for Arduino\nBy InnovationGarage AS\n" BANNER

// Uncomment to support the DEC special character set (drawing characters)
// Note that you must use a font that supports this too for this to work.
#define DEC_SPECIAL_CHARACTER_SET

// Use a full screen framebuffer. This is faster than a partial
// buffer, but uses more RAM.
#define DISPLAY_BUFFER_FULL

// Comment out to only use the internal Arduino buffer of 64 bytes
// Note: The draw loop takes ~50ms, and you need enough buffer to not
// drop bytes during that time, e.g. at 115200baud 8N1: 115200/9 =
// 12800bytes/s: 12800/64 = 200buffers/s: 1/200 = 0.005s/buffer.
// that is, the Arduino buffer is filled in 5ms alone!
#define SERIAL_BUFFER_SIZE 1024




/* End build configuration */
