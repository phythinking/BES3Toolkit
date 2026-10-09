#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:   Dr. ZHOU Xiang
# Date:     2022-03-13
# WALL⋅E will setup BOSS as user's request.

import argparse
import sys
sys.path.append('/workfs2/bes/zhoux/BES3Toolkit/iRobot/classes')
from setupboss import SetupBOSS

if __name__ == '__main__':
    # set arguments
    parser = argparse.ArgumentParser(description='setup BOSS with input version')
    parser.add_argument('--bossversion', type=str, required=True, help='BOSS Version, e.g. 7.0.9')
    args = parser.parse_args()
    # WALL⋅E will setup BOSS as user;s requst.
    WALLE_SetupBOSS = SetupBOSS(args.bossversion)
    WALLE_SetupBOSS.install()


