#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:   Dr. ZHOU Xiang
# Date:     2022-03-13
# WALL⋅E will compile algorithm as user's request.

import argparse
import sys
sys.path.append('/workfs2/bes/zhoux/BES3Toolkit/iRobot/classes')
from compileAlgorithm import CompileAlgorithm

if __name__ == '__main__':
    # set arguments
    parser = argparse.ArgumentParser(description='Create Algorithm with input BOSS Version and Algorithm Name and Algorithm Version')
    parser.add_argument('--bossversion', type=str, required=True, help='BOSS Version, e.g. 7.0.9')
    parser.add_argument('--algname', type=str, required=True, help='Algorithm Name, e.g. JpsiTo2pi4gamAlg')
    parser.add_argument('--algversion', type=str, required=True, help='Algorithm Version, e.g. 00-00-01')
    args = parser.parse_args()
    # WALL⋅E will setup BOSS as user;s requst.
    WALLE_CompileAlg = CompileAlgorithm(args.bossversion, args.algname, args.algversion)
    WALLE_CompileAlg.compile()
