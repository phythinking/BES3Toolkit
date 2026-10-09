#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:   Dr. ZHOU Xiang
# Date:     2020-03-04
# Modified: 2021-06-29
# Version:  2.00
# WALL⋅E will install setup BOSS as the version given by EVE 
# The code is tranferred from tcsh to python3
# Note that the containers do not support python3

import os

from datetime import datetime

from textcolor import TextColor

class WALLE:
    """Overall class for WALL⋅E to do jobs according to EVE's requests."""

    def __init__(self):
        """Initial WALL⋅E"""
        self.name         = 'WALL⋅E' # Waste Allocation Load Lifter Earth-Class
        self.version	  = '2.00'
        self.girlfriend   = 'EVE'    # Extraterrestrial Vegetation Evaluator
        self.user      = os.environ['USER']

    def __str__(self):
        """Return a self-introduction from WALL⋅E."""
        intro_text = []
        if self.user == 'zhoux':
            intro_text.append(TextColor.blue('Hi '))
            intro_text.append(TextColor.bold(TextColor.deepred('Dr. Zhou')))
            intro_text.append(TextColor.blue('! Welcome back!\n'))
            intro_text.append(TextColor.magenta(self.name))
            intro_text.append('-')
            intro_text.append(TextColor.green(self.version))
            intro_text.append(TextColor.blue(' is ready!'))
        else:
            intro_text.append('Hello ')
            intro_text.append(TextColor.bold(self.user))
            intro_text.append('!\nI am ')
            intro_text.append(TextColor.magenta(self.name))
            intro_text.append('-')
            intro_text.append(TextColor.red(self.version))
            intro_text.append('.\nMy girlfriend is ')
            intro_text.append(TextColor.green(self.girlfriend))
            intro_text.append('.')
            intro_text.append(TextColor.bold(TextColor.deepred('\nWhere is EVE?')))
        return "".join(intro_text)

if __name__ == '__main__':
    # Make WALL⋅E wake up and introduce himself.
    WALLE_robot = WALLE()
    print(WALLE_robot)
#    WALLE_robot.initialize()

