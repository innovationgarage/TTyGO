/* Build configuration */

// Hardware platform, uncomment exactly one of these
#include "config.ttgo.h"
//#include "config.arduboy.h"

// Communication speed
#define SERIAL_BAUDS DEFAULT_SERIAL_BAUDS

// ************************************************
// **** On Screen Keyboard configuration  *********
// ************************************************

// Check hardware mapping configuration for tunning the speed of they key repeats when holding and scrolling

// Comment out to disable the on screen keyboard
// Set to button index to bring up the OSK
#define ON_SCREEN_KEYBOARD 1
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
#define WIDECHAR DEFAULT_WIDECHAR

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall
#define LCD_FONT u8g2_font_4x6_mf

// WIDTH should be >= u8g2.getDisplayWidth() / font.char_width
// HEIGHT should be >= u8g2.getDisplayHeight() / font.char_height
#define TERMINAL_MAX_WIDTH DEFAULT_TERMINAL_MAX_WIDTH
#define TERMINAL_MAX_HEIGHT DEFAULT_TERMINAL_MAX_HEIGHT

// Max length of programmable button strings in bytes
#define BUTTON_STRLEN DEFAULT_BUTTON_STRLEN

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

// Set to 1 to enable the DEC special character set (drawing characters)
// Note that you must use a font that supports this too for this to work.
#define DEC_SPECIAL_CHARACTER_SET DEFAULT_DEC_SPECIAL_CHARACTER_SET

// Display buffer size: 1 for a single page, 2 for speedup using 2
// pages, F for a full screen framebuffer. A full screen buffer is the
// fastest option, but uses considerably more RAM.
#define DISPLAY_BUFFER DEFAULT_DISPLAY_BUFFER

// Size of the serial port buffer in bytes.
// Set to 0 to only use the internal Arduino buffer of 64 bytes
// Note: The draw loop takes ~50ms, and you need enough buffer to not
// drop bytes during that time, e.g. at 115200baud 8N1: 115200/9 =
// 12800bytes/s: 12800/64 = 200buffers/s: 1/200 = 0.005s/buffer.
// that is, the Arduino buffer is filled in 5ms alone!
#define SERIAL_BUFFER_SIZE DEFAULT_SERIAL_BUFFER_SIZE

// Initial button bindings
// Strings for btn1 short click, btn1 long click, btn2 short click, btn2 long click...
#define BUTTON_BINDINGS "\x1b[A", "\x1b[C", "\n", "\x1b", "\x1b[B", "\x1b[D"

/* End build configuration */
