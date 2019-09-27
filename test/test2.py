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
