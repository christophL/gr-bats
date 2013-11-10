#
# Copyright 2008,2009 Free Software Foundation, Inc.
#
# This application is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This application is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

# The presence of this file turns this directory into a Python package

'''
This is the GNU Radio DIGIMODES module. Place your Python package
description here (python/__init__.py).
'''

# ----------------------------------------------------------------
# Temporary workaround for ticket:181 (swig+python problem)
import sys
_RTLD_GLOBAL = 0
try:
    from dl import RTLD_GLOBAL as _RTLD_GLOBAL
except ImportError:
    try:
	from DLFCN import RTLD_GLOBAL as _RTLD_GLOBAL
    except ImportError:
	pass

if _RTLD_GLOBAL != 0:
    _dlopenflags = sys.getdlopenflags()
    sys.setdlopenflags(_dlopenflags|_RTLD_GLOBAL)
# ----------------------------------------------------------------


# import swig generated symbols into the digimodes namespace
#from digimodes_swig import *

varicodes = {

### 1st col

"0"  :    "1010101011",
"1"  :    "1011011011",
"2"  :    "1011101101",
"3"  :    "1101110111",
"4"  :    "1011101011",
"5"  :    "1101011111",
"6"  :    "1011101111",
"7"  :    "1011111101",
"8"  :    "1011111111",
"9"  :    "11101111"  ,
"\n" :    "11101"     ,
"11" :    "1101101111",
"12" :    "1011011101",
"\r" :    "11111"     ,
"14" :    "1101110101",
"15" :    "1110101011",
"16" :    "1011110111",
"17" :    "1011110101",
"18" :    "1110101101",
"19" :    "1110101111",
"20" :    "1101011011",
"21" :    "1101101011",
"22" :    "1101101101",
"23" :    "1101010111",
"24" :    "1101111011",
"25" :    "1101111101",
"26" :    "1110110111",
"27" :    "1101010101",
"28" :    "1101011101",

### 2nd col

"29" :     "1110111011",
"30" :     "1011111011",
"31" :     "1101111111",
" "  :     "1",
"!"  :     "111111111",
"\"" :     "101011111",
"#"  :     "111110101",
"$"  :     "111011011",
"%"  :     "1011010101",
"&"  :     "1010111011",
"'"  :     "101111111",
"("  :     "11111011",
")"  :     "11110111",
"*"  :     "101101111",
"+"  :     "111011111",
","  :     "1110101",
"-"  :     "110101",
"."  :     "1010111",
"/"  :     "110101111",
"0"  :     "10110111",
"1"  :     "10111101",
"2"  :     "11101101",
"3"  :     "11111111",
"4"  :     "101110111",
"5"  :     "101011011",
"6"  :     "101101011",
"7"  :     "110101101",
"8"  :     "110101011",
"9"  :     "110110111",

### 3rd col
":"  :     "11110101",
";"  :     "110111101",
"<"  :     "111101101",
"="  :     "1010101",
">"  :     "111010111",
"?"  :     "1010101111",
"@"  :     "1010111101",
"A"  :     "1111101",
"B"  :     "11101011",
"C"  :     "10101101",
"D"  :     "10110101",
"E"  :     "1110111",
"F"  :     "11011011",
"G"  :     "11111101",
"H"  :     "101010101",
"I"  :     "1111111",
"J"  :     "111111101",
"K"  :     "101111101",
"L"  :     "11010111",
"M"  :     "10111011",
"N"  :     "11011101",
"O"  :     "10101011",
"P"  :     "11010101",
"Q"  :     "111011101",
"R"  :     "10101111",
"S"  :     "1101111",
"T"  :     "1101101",
"U"  :     "101010111",
"V"  :     "110110101",

### 4t col
"W"  :     "101011101",
"X"  :     "101110101",
"Y"  :     "101111011",
"Z"  :     "1010101101",
"["  :     "111110111",
"\"" :     "111101111",
"]"  :     "111111011",
"^"  :     "1010111111",
"_"  :     "101101101",
"`"  :     "1011011111",
"a"  :     "1011",
"b"  :     "1011111",
"c"  :     "101111",
"d"  :     "101101",

### 5th col
"e"  :     "11",
"f"  :     "111101",
"g"  :     "1011011",
"h"  :     "101011",
"i"  :     "1101",
"j"  :     "111101011",
"k"  :     "10111111",
"l"  :     "11011",
"m"  :     "111011",
"n"  :     "1111",
"o"  :     "111",
"p"  :     "111111",
"q"  :     "110111111",
"r"  :     "10101",

### 6th col
"s"  :     "10111",
"t"  :     "101",
"u"  :     "110111",
"v"  :     "1111011",
"w"  :     "1101011",
"x"  :     "11011111",
"y"  :     "1011101",
"z"  :     "111010101",
"{"  :     "1010110111",
"|"  :     "110111011",
"}"  :     "1010110101",
"~"  :     "1011010111",
"127":     "1110110101"
}

# import any pure python here
#
from psk31_decoder import psk31_decoder






# ----------------------------------------------------------------
# Tail of workaround
if _RTLD_GLOBAL != 0:
    sys.setdlopenflags(_dlopenflags)      # Restore original flags
# ----------------------------------------------------------------
