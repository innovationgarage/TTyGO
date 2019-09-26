#! /usr/bin/python

import sys
import os
import unittest
import serial

term = serial.Serial(os.environ.get("TTGOTERM", "/dev/ttyUSB0"), baudrate=int(os.environ.get("TTGOTERM_SPEED", 9600)), timeout=3.0)

def rd():
    return term.read(1)
def wr(s):
    term.write(s)

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

    def test_cursor_relative(self):
        wr("\x1b[2;2H\x1b[3B\x1b[1A\x1b[3C\x1b[1DX")
        o = strw(self.orig, 4, 4, "X").strip()
        r = readscreen().strip()
        self.assertEqual(o, r, "\n%s\n!=\n%s" % (o, r))
        
    def test_cursor_absolute(self):
        wr("\x1b[3;9HX")
        o = strw(self.orig, 9, 3, "X").strip()
        r = readscreen().strip()
        self.assertEqual(o, r, "\n%s\n!=\n%s" % (o, r))
