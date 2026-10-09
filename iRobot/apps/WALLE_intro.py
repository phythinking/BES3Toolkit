#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:   Dr. ZHOU Xiang
# Date:     2022-03-13
# Version:  2.00
# WALL⋅E will introduce himself 

import sys
sys.path.append('/workfs2/bes/zhoux/BES3Toolkit/iRobot/classes')
from WALLE import WALLE

if __name__ == '__main__':
    # Make WALL⋅E wake up and introduce himself.
    WALLE_intro = WALLE()
    print(WALLE_intro)
#    WALLE_robot.initialize()


