# Using the Arduino IDE
## Required libraries

From the Arduino library manager:
* OneButton
* U8g2
* ESP8266Scheduler

# Automated build from the command line

    bash install.sh

This will install [arduino-cli](https://github.com/arduino/arduino-cli) if not already installed, download the esp8266 board as well as all required libraries, build TTyGO and upload it to your board (by default via /dev/ttyUSB0, set TTGOTERM to the device to override).
