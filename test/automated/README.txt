Test cases for TTGOTerm. To run, first upload the firmware to the TTGO device, then run the testcases with

    nose2 -v

If you are using a non-standard serial port, or if your TTGO is
configured to use a non-standard baudrate, you can override the
defaults by setting these environment variables prior to running nose:

    export TTGOTERM=/dev/ttyUSB0
    export TTGOTERM_SPEED=9600

