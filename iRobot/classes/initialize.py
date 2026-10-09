#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Initialize user's home
# Author: Dr. ZHOU Xiang
# Email : xiangzhou@whu.edu.cn
# Data  : 2021-07-14

import os
import shutil
import subprocess
import distutils.dir_util

from datetime import datetime

from preferences import Preferences
from textcolor    import TextColor
from toolkit      import Toolkit

class Initialize:
    """A class to initialize earth environment."""
    def __init__(self):
        self.user      = os.environ['USER']
        self.home      = os.environ['HOME']
        self.shellname = os.environ['SHELL'].split('/')[-1]# '/bin/tcsh' -> 'tcsh'

    def initialize(self, firstday = False):
        runflag = False
        if self.shellname != 'tcsh':
            print('WALL⋅E can only live in tcsh')
            with open(self.logfile, 'a') as a:
                a.write(f'WALL⋅E self-check failed.\n')
                a.write(f'Please contact helpdesk to change shell to tcsh.\n')
        return runflag
        tcshrc_file   = os.path.join(self.home, '.tcshrc')
        vimrc_file    = os.path.join(self.home, '.vimrc')
        source_tcshrc = os.path.join(self.preferences.iRobot_dir, 'tcshrc')
        source_vimrc  = os.path.join(self.preferences.iRobot_dir, 'vimrc')
        if not firstday:
            if not os.path.isfile(tcshrc_file):
                print(TextColor.red('WALL⋅E cannot find .tcshrc in home!'))
            if not os.path.isfile(vimrc_file):
                print(TextColor.red('WALL⋅E cannot find .vimrc in home!'))
        else: # firstday!
            if self.user != 'zhoux':
                # backup .tcshrc and .vimrc if they exist
                now = datetime.now().strftime('%Y%m%d%H%M%S')
                if os.path.isfile(tcshrc_file):
                    print(TextColor.magenta('WALL⋅E backups .tcshrc in home!'))
                    shutil.copyfile(tcshrc_file, tcshrc_file + '.' + now)
                if os.path.isfile(vimrc_file):
                    print(TextColor.magenta('WALL⋅E backups .vimrc in home!'))
                    shutil.copyfile(vimrc_file, vimrc_file + '.' + now)
                # create .tcshrc and .vimrc in home
                print(TextColor.blue('WALL⋅E makes .tcshrc in home!'))
                shutil.copyfile(source_tcshrc, tcshrc_file)
                print(TextColor.blue('WALL⋅E makes .vimrc in home!'))
                shutil.copyfile(source_vimrc , vimrc_file)
                runflag = True
            else:
                runflag = True
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'WALL⋅E self-check succeeded.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'WALL⋅E self-check failed.\n')
        return runflag

if __name__ == '__main__':
    # Initialize user's home
    init = Initialize()
    init.initialize()

