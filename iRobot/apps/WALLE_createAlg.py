#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:   Dr. ZHOU Xiang
# Date:     2022-03-13
# WALL⋅E will create algorithm as user's request.

import argparse
import sys
sys.path.append('/workfs2/bes/zhoux/BES3Toolkit/iRobot/classes')
from createAlgorithm import CreateAlgorithm

if __name__ == '__main__':
    # set arguments
    parser = argparse.ArgumentParser(description='Create Algorithm with input BOSS Version and Algorithm Name')
    parser.add_argument('--bossversion', type=str, required=True, help='BOSS Version, e.g. 7.0.9')
    parser.add_argument('--algname', type=str, required=True, help='Algorithm Name, e.g. JpsiTo2pi4gamAlg')
    args = parser.parse_args()
    # WALL⋅E will setup BOSS as user;s requst.
    WALLE_CreateAlg = CreateAlgorithm(args.bossversion, args.algname)
    WALLE_CreateAlg.create()
