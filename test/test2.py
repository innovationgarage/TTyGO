# -*- coding: utf-8 -*-

import sys
import os
import tty
import termios
import threading

stdoutfd = sys.stdout.fileno()
stdinfd = sys.stdin.fileno()
old_settings = termios.tcgetattr(sys.stdin)
tty.setcbreak(stdinfd)
def rd():
    return os.read(stdinfd, 1)
def wr(s):
    sys.stdout.write(s)
    sys.stdout.flush()

class Reader(threading.Thread):
    def run(self, *arg, **kw):
        while True:
            c = rd()
            sys.stderr.write(c)
            sys.stderr.flush()

Reader().start()

wr("\x1bc")

wr("Hej")
def hexify(s):
    return "".join(hex(ord(i))[2:] for i in s.encode("utf-8"))
wr("\x1bP0;0;0/%s;2/%s;4/%s\x9c" % (hexify(u"hejhop"), hexify(u"nana"), hexify(u"åä")))
wr("Tryck på en knapp")

#wr("\x1b(0\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78");
#wr("\x1b(Baaaaa");
          
# wr("Räksmörgås")

# wr("""\
# ********************************
# *                              *
# *                              *
# *                              *
# *                              *
# * Räksmörgås                   *
# ********************************
# """)

# wr("\x1b[5A\x1b[2COn the first line")
# wr("\x1b[1E\x1b[3GOn the second line")
# wr("\x1b[4;3HOn the muahahaha")
# wr("\x1b[1E\x1b[2COn the forth line")
# wr("\x1b[5;4H")
# wr("\x1b[1A\x1b[1DOn the third line")
# wr("\x1b[7;1H")

# wr("\x1b[2;2H")
