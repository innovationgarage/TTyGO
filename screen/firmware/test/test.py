#! /usr/bin/python

import sys
import os
import tty
import termios

stdoutfd = sys.stdout.fileno()
stdinfd = sys.stdin.fileno()
old_settings = termios.tcgetattr(sys.stdin)
tty.setcbreak(stdinfd)
def rd():
    return os.read(stdinfd, 1)
def wr(s):
    sys.stdout.write(s)
    sys.stdout.flush()

# Reset screen
wr("\x1bc")

wr("""\
********************************
*                              *
*                              *
*                              *
*                              *
********************************
""")

wr("\x1b[5A\x1b[2COn the first line")
wr("\x1b[1E\x1b[3GOn the second line")
wr("\x1b[4;3HOn the muahahaha")
wr("\x1b[1E\x1b[2COn the forth line")
wr("\x1b[5;4H")
wr("\x1b[1A\x1b[1DOn the third line")
wr("\x1b[7;1H")

wr("\x1b[2;2H")

wr("\x1b[0i")

while True:
    c1 = rd()
    #if c1 == "x":
    #    termios.tcsetattr(stdinfd, termios.TCSANOW, old_settings)
    #    break
    if c1 == "\x1b":
        c2 = rd()

        if c2 == "[":
            c3 = rd()
      
            if c3 == "A":
                wr("\x1b[2G ")
                wr("\x1b[A\x1b[2GX")
            elif c3 == "B":
                wr("\x1b[2G ")
                wr("\x1b[B\x1b[2GX")
            else:
                print("c3", repr(c3))
        else:
            print("c2", repr(c2))
    #elif c1 == "\n" or c1 == "\r":
    #    wr(">>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    #    wr("\x1b[2G>")
    else:
        sys.stderr.write(c1)
        sys.stderr.flush()
