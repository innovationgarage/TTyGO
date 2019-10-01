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
