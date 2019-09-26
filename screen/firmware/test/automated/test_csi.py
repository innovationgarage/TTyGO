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

    linelen = len(orig.split("\n")[0])

    def assertScreen(self, shouldbe, actual):
        def clean(s):
            return "\n".join(l.rstrip() for l in s.rstrip().split("\n"))
        shouldbe = clean(shouldbe)
        actual = clean(actual)
        self.assertEqual(shouldbe, actual, "\n%s\n!=\n%s" % (shouldbe, actual))
    
    def setUp(self):
        wr("\x1bc")
        wr(self.orig)

    def test_insert_blank(self):
        wr("\x1b[1;1H\x1b[4@")
        o = strw(self.orig, 1, 1, "    ")
        r = readscreen()
        self.assertScreen(o, r)

    def test_cursor_relative(self):
        wr("\x1b[2;2H\x1b[3B\x1b[1A\x1b[3C\x1b[1DX")
        o = strw(self.orig, 4, 4, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_cursor_next_prev_line(self):
        wr("\x1b[3;9H\x1b[1EX")
        wr("\x1b[3;9H\x1b[1FY")
        o = strw(strw(self.orig, 1, 4, "X"), 1, 2, "Y")
        r = readscreen()
        self.assertScreen(o, r)
        
    def test_cursor_char_absolute(self):
        wr("\x1b[3;9H\x1b[4GX")
        o = strw(self.orig, 4, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)
        
    def test_cursor_absolute(self):
        wr("\x1b[3;9HX")
        o = strw(self.orig, 9, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_cursor_tab_forward(self):
        wr("\x1b[3;1H\x1b[2IX")
        o = strw(self.orig, 17, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_clear_below_cursor(self):
        wr("\x1b[3;1H\x1b[0J")
        o = "\n".join(self.orig.split("\n")[:2])
        r = readscreen()
        self.assertScreen(o, r)

    def test_clear_above_cursor(self):
        wr("\x1b[3;1H\x1b[1J")
        o = "\n".join([" " * self.linelen, " " * self.linelen] + self.orig.split("\n")[2:])
        r = readscreen()
        self.assertScreen(o, r)

    def test_clear_to_end_of_line(self):
        wr("\x1b[3;9HY\x1b[3;8H\x1b[0KX")
        o = strw(self.orig, self.linelen, 3, " ")
        o = strw(o, 9, 3, " ")
        o = strw(o, 8, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_clear_to_beginning_of_line(self):
        wr("\x1b[3;8HY\x1b[3;9H\x1b[1KX")
        o = strw(self.orig, 1, 3, " ")
        o = strw(o, 8, 3, " ")
        o = strw(o, 9, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_clear_line(self):
        wr("\x1b[3;8HX\x1b[2KX")
        o = strw(self.orig, 1, 3, " ")
        o = strw(o, self.linelen, 3, " ")
        o = strw(o, 8, 3, " ")
        o = strw(o, 9, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_insert_lines(self):
        wr("\x1b[3;9H\x1b[2LX")
        lines = self.orig.split("\n")
        o = "\n".join(lines[:2] + [" " * self.linelen, " " * self.linelen] + lines[2:])
        o = strw(o, 9, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_delete_lines(self):
        wr("\x1b[4;9HA\x1b[3;9HB\x1b[3;9H\x1b[1MX")
        lines = self.orig.split("\n")
        o = "\n".join(lines[:2] + lines[3:])
        o = strw(o, 9, 3, "X")
        r = readscreen()
        self.assertScreen(o, r)

    def test_delete_characters(self):
        wr("\x1b[3;9HABCD\x1b[3;9H\x1b[2PX")
        lines = self.orig.split("\n")
        o = "\n".join(lines[:2] + lines[3:])
        o = strw(self.orig, 9, 3, "XD")
        o = strw(o, 30, 3, "|")
        o = strw(o, self.linelen, 3, " ")
        r = readscreen()
        self.assertScreen(o, r)

    def test_scroll_up(self):
        wr("\x1b[2S")
        lines = self.orig.strip().split("\n")
        o = "\n".join(lines[2:])
        r = readscreen()
        self.assertScreen(o, r)

    def test_scroll_down(self):
        wr("\x1b[2T\x1b[1;1HX")
        o = "X\n\n" + self.orig
        r = readscreen()
        self.assertScreen(o, r)

    def test_erase_character(self):
        wr("\x1b[3;8HABCD\x1b[3;8H\x1b[3XY")
        o = strw(self.orig, 8, 3, "Y")
        o = strw(o, 11, 3, "D")
        r = readscreen()
        self.assertScreen(o, r)

    def test_cursor_tab_backward(self):
        wr("\x1b[3;17HA\x1b[1ZB")
        o = strw(self.orig, 17, 3, "A")
        o = strw(o, 9, 3, "B")
        r = readscreen()
        self.assertScreen(o, r)
