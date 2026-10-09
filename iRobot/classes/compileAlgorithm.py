#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Compile Algorithm package for BESIII analysis
# Author: Dr. ZHOU Xiang
# Email : xiangzhou@whu.edu.cn
# Data  : 2022-03-09

import os
import shutil
import subprocess
import distutils.dir_util

from datetime import datetime

from environments import Environments
from textcolor    import TextColor
from toolkit      import Toolkit

class CompileAlgorithm:
    """A class to compile algorithm package."""
    def __init__(self, bossversion, algname, algversion):
        self.bossversion       = bossversion
        self.algname           = algname
        self.algversion        = algversion
        self.environments      = Environments(self.bossversion)
        self.toolkit           = Toolkit(self.bossversion)
        self.short_bossversion = self.environments.short_bossversion
        self.container         = self.environments.container
        self.logfile           = self.environments.logfile
        self.cmthome_dir       = self.environments.cmthome_dir
        self.work_dir          = self.environments.work_dir
        self.TestRelease_dir   = self.environments.TestRelease_dir
        self.workfs            = self.environments.workfs
        self.cmthome           = self.environments.cmthome
        self.TestRelease       = self.environments.TestRelease
        self.home              = self.environments.home
        self.user              = self.environments.user
        self.bossbin_dir       = self.environments.bossbin_dir
        self.Boss              = self.environments.Boss
        self.iRobot_dir        = self.environments.preferences.iRobot_dir

    def compile(self):
        # set runflag
        runflag = False
        # write log
        self._initial_runflag()
        runflag = self._modify_requirements()

        # change requirements in TestRelease 

    def _initial_runflag(self):
        with open(self.logfile, 'a') as a:
            a.write('################################\n')
            a.write(datetime.now().strftime('%Y/%m/%d %H:%M:%S'))
            a.write('\n')
            a.write(f'WALL⋅E starts compiling {self.algname}\n')

    def _modify_requirements(self):
        runflag = False
        # find requirements in TestRelease and enter into its directory
        TestRelease_dir = os.path.join(self.workfs, 'work.' + self.bossversion, 'TestRelease') 
        file_info_list = self.toolkit._findfile(TestRelease_dir, 'requirements')
        os.chdir(file_info_list[1])
        source_keywords = ['use ' + self.algname]
        addbelow_source_keywords = ['use VertexFit ']
        target_line = 'use ' + self.algname + '     ' + self.algname + '-' + self.algversion + '\n'
        lineflag = self.toolkit._replace_line(file_info_list[2], source_keywords, target_line)
        if not lineflag:
            lineflag = self.toolkit._replace_line(file_info_list[2], addbelow_source_keywords, target_line, add = True)
        # cmt broadcost cmt config
        # cmt broadcost cmt make
        algcmt_dir = os.path.join(self.workfs, 'work.' + self.bossversion, self.algname, self.algname + '-' + self.algversion, 'cmt')
        print(algcmt_dir)
        os.chdir(algcmt_dir)
        # get time
        now = datetime.now().strftime('%Y%m%d%H%M%S')
        tcsh_file = f'config+{now}.csh'
        tcsh_script1 = os.path.join(self.cmthome_dir, 'setupCMT.csh')
        tcsh_script2 = os.path.join(self.cmthome_dir, 'setup.csh')
        tcsh_script3 = os.path.join(file_info_list[1], 'setup.csh')
        with open(tcsh_file, 'w') as w:
            w.write('#!/bin/tcsh -f\n')
            w.write(f'source {tcsh_script1}\n')
            w.write(f'source {tcsh_script2}\n')
            w.write(f'source {tcsh_script3}\n')
            w.write('cmt config\n')
            w.write('cmt make\n')
        os.system(f'chmod +x {tcsh_file}')
        os.system(f'hep_container exec {self.container} ./{tcsh_file}') 
        print('------------------------------------------')
        self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
        self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
        self.toolkit._replace_keyword('setup.csh'  , 'cmt ', 'cmt -disable_warnings ')
        self.toolkit._replace_keyword('setup.sh'   , 'cmt ', 'cmt -disable_warnings ')
        os.remove(tcsh_file)
        runflag = True
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.algname}-{self.algversion} is compiled.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'Compiling {self.algname}-{self.algversion} failed.\n')
        return runflag

if __name__ == '__main__':
    WALLE_compileAlgorithm = CompileAlgorithm('7.0.9', 'testAlg', '00-00-01')
    WALLE_compileAlgorithm.compile()
