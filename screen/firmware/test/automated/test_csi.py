#! /usr/bin/python

import sys
import os
import tty
import termios
import unittest

term = os.open(os.environ.get("TTGOTERM", "/dev/ttyUSB0"), os.O_RDWR)

tty.setcbreak(term)
def rd():
    return os.read(term, 1)
def wr(s):
    os.write(term, s)

def strw(s, x, y, c):
    y -= 1
    x -= 1
    lines = s.split("\n")
    return "\n".join(lines[:y] + [lines[y][:x] + c + lines[y][x+len(c):]] + lines[y+1:])

def readscreen():
    wr("\x1b[0i")
    buf = ""
    while True:
        buf += rd()
        if "\x1b[201~" in buf:
            buf = buf.split("\x1b[200~")[1]
            buf = buf.split("\x1b[201~")[0]
            return buf

class TestCSI(unittest.TestCase):
    orig = """\
12345678901234567890123456789012
2                              |
3                              |
4                              |
+------------------------------+
"""
    
    def setUp(self):
        wr("\x1bc")
        wr(self.orig)
 
    def test_cursor_absolute(self):
        wr("\x1b[3;9HX")
        o = strw(self.orig, 9, 3, "X").strip()
        r = readscreen().strip()
        self.assertEqual(o, r, "\n%s\n!=\n%s" % (o, r))
