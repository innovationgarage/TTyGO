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

def dbg(s):
    sys.stderr.write(s + "\n")
    sys.stderr.flush()
    
# Reset screen
wr("\x1bc")

wr("""\
********************************
*                              *
*                              *
*                              *
*                              *
* Print screen                 *
* Exit                         *
********************************
""")

wr("\x1b[7A\x1b[2COn the first line")
wr("\x1b[1E\x1b[3GOn the second line")
wr("\x1b[4;3HOn the muahahaha")
wr("\x1b[1E\x1b[2COn the forth line")
wr("\x1b[5;4H")
wr("\x1b[1A\x1b[1DOn the third line")

wr("\x1b[2;2H")

pos = 2
while True:
    c1 = rd()
    if c1 == "\x1b":
        dbg("ESC")
        c2 = rd()
        
        if c2 == "[":
            dbg("CSI")
            c3 = rd()
      
            if c3 == "A":
                dbg("CSI UP")
                pos -= 1
                wr("\x1b[2G ")
                wr("\x1b[A\x1b[2GX")
            elif c3 == "B":
                dbg("CSI DOWN")
                pos += 1
                wr("\x1b[2G ")
                wr("\x1b[B\x1b[2GX")
            else:
                dbg("Unknown CSI " + c3)
        else:
            dbg("Unknown ESC " + c2)
    elif c1 == "\n" or c1 == "\r":
        dbg("ENTER")
        if pos == 6:
            dbg("PRINT SCREEN")
            wr("\x1b[0i")
        elif pos == 7:
            dbg("EXIT")
            termios.tcsetattr(stdinfd, termios.TCSANOW, old_settings)
            break
        else:
            dbg("SELECT")
            wr(">>>>>>>>>>>>>>>>>>>>>>>>>>>>")
            wr("\x1b[2G>")
    else:
        sys.stderr.write(c1)
        sys.stderr.flush()
