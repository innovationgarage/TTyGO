# Buttons

There are three physical buttons supported, each with two logical
functions on shift click and long press. The buttons act as [VT220
4.15 User Defined
Keys](https://vt100.net/docs/vt220-rm/chapter4.html#S4.15).

However they are numbered from 0, not 17 as on the original VT220. The
logical functions (click/long press) are thus programmed by
referencing button number 0/1, 2/3 and 4/5 respectively.

Furthermore, on reset however, the these keys are bound to
ARROWUP/ARROWLEFT, ENTER/ESC and to ARROWDOWN/ARROWRIGHT respectively
which should work pretty well for a lot of menu applications.

# Print screen

CSI 0 i (Media copy) will send the screen content back over the serial
line, bracketed in [paste
mode](https://www.xfree86.org/current/ctlseqs.html#Bracketed%20Paste%20Mode)
CSI 200 ~ and CSI 201 ~. Note: Lines are truncated after the last
printable non-empty character on the line. By definition, only
printable characters and newline will be sent back - tab etc will have
already been converted to spaces.

This is effectively a memory dump of the terminal buffer. It is used
in the unit tests (tests/automated), but can be used in applications
as well - it is not intended to be an internal only function.

# Character encoding

There is no Latin-1 / ISO 8859 support - the terminal will happily
accept you switching to e.g. swedish character encoding, but will
still only ever interpret non-ascii characters as UTF-8 (or [Dec
Special Character
Set](http://fileformats.archiveteam.org/wiki/DEC_Special_Graphics_Character_Set)).

# Screen size

The screen size is rarely 80x24 characters. The current screen size
can be queried [CSI 18
t](https://www.xfree86.org/current/ctlseqs.html#VT100%20Mode), which
will be replied to with CSI 8; height; width t.
