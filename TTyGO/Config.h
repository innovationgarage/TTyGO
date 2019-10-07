/* Build configuration */

// Hardware mapping configuration
#define BTN_LEFT 12
#define BTN_MID 14
#define BTN_RIGHT 13

// Comment the line below to use the "fake" scheduler
//#define USE_ESP8266SCHEDULER

#define LCD_CLK 5
#define LCD_SDA 4
#define LCD_FPS_TARGET 60

#define SERIAL_BAUDS 115200


// Software feature configuration

// Store strings constants in flash rather than RAM
#define FLASH_STRINGS 1

// Length of UTF-8 multi-byte sequences to support. If 1, only ASCII
// is supported. Max is 4.
// Note that font support for your codepoints is also required!
#define WIDECHAR 2

// Font from https://github.com/olikraus/u8g2/wiki/fntlistall
#define lcd_font u8g2_font_4x6_mf

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

// Use https://github.com/nrwiersma/ESP8266Scheduler for task scheduling
// Note: Only available on the ESP platform
// #define USE_ESP8266SCHEDULER

/* End build configuration */
