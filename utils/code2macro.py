#!/usr/bin/env python

# takes a piece of code and escapes every line for fast macro cosy and paste
import sys


if __name__ == '__main__':

    print("please copy and paste your charge discharge data.\n"
      "To end recording Press Ctrl+d on Linux/Mac on Crtl+z on Windows")
    lines = []

    with open(0) as f:

        line = f.readline()
        while line:
            if line.strip().upper() == "/END":
                break
            lines.append( line.rstrip() )
            line = f.readline()
                  
    justify_size = max(len(x) for x in lines)

    for first in lines:
        print( first + '\\'.rjust( justify_size - len(first) + 1) )
    
