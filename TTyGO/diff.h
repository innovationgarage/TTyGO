12,13c12,13
< // Up, A, Down, Left, Right, B
< #define BUTTONS BUTTON(A0, true) BUTTON(7, true) BUTTON(A3, true) BUTTON(A2, true) BUTTON(A1, true) BUTTON(8, true)
---
> // Left, Middle, Right
> #define BUTTONS BUTTON(12, true) BUTTON(14, true) BUTTON(13, true)
22c22
< #define DISPLAY_CONTROLLER SSD1306
---
> #define DISPLAY_CONTROLLER SH1106 
24c24
< #define DISPLAY_COMM 4W_HW_SPI
---
> #define DISPLAY_COMM HW_I2C
26c26
< #define DISPLAY_ARGS U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6
---
> #define DISPLAY_ARGS U8G2_R0, /* reset=*/ U8X8_PIN_NONE
30d29
< 
34a34,35
> // Use https://github.com/nrwiersma/ESP8266Scheduler for task scheduling
> // Note: Only available on the ESP platform
36c37
< //#define USE_ESP8266SCHEDULER
---
> #define USE_ESP8266SCHEDULER
40c41
< // #define ESPI2C
---
> #define ESPI2C
72c73
< #define WIDECHAR 1
---
> #define WIDECHAR 2
80c81
< #define TERMINAL_MAX_HEIGHT 12
---
> #define TERMINAL_MAX_HEIGHT 24
83c84
< #define BUTTON_STRLEN 5
---
> #define BUTTON_STRLEN 10
101c102
< // #define DEC_SPECIAL_CHARACTER_SET
---
> #define DEC_SPECIAL_CHARACTER_SET
106,110c107
< #define DISPLAY_BUFFER 1
< 
< // Use https://github.com/nrwiersma/ESP8266Scheduler for task scheduling
< // Note: Only available on the ESP platform
< // #define USE_ESP8266SCHEDULER
---
> #define DISPLAY_BUFFER F
117c114
< // #define SERIAL_BUFFER_SIZE 1024
---
> #define SERIAL_BUFFER_SIZE 1024
121c118,119
< #define BUTTON_BINDINGS "\x1b[A", "\x1b[C", "\n", "\x1b", "\x1b[B", "\x1b[D" 
---
> #define BUTTON_BINDINGS "\x1b[A", "\x1b[C", "\n", "\x1b", "\x1b[B", "\x1b[D"
> 
