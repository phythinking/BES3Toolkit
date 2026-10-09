#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Setup BOSS for BESIII analysis
# Author: Dr. ZHOU Xiang
# Email : xiangzhou@whu.edu.cn
# Data  : 2021-07-14

import os
import shutil
import subprocess
import distutils.dir_util

from datetime import datetime

from environments import Environments
from textcolor    import TextColor
from toolkit      import Toolkit

class SetupBOSS:
    """A class to setup BOSS environment."""
    def __init__(self, bossversion):
        self.bossversion       = bossversion
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

    def install(self):
        # initialize runflag
        runflag = False
        # write log
        self._initial_runflag()
        # make cmthome directory
        self.toolkit._makedir(os.path.join(self.workfs, 'cmthome'))
        # copy cmthome-bossversion package
        runinfo_list = self.toolkit._prepare_package(self.cmthome, self.cmthome_dir)
        # configure cmthome-bossversion
        runflag = runinfo_list[0] # renew runflag
        if runflag:
            self._config_cmthome()
            # make work.bossversion directory and renew runflag
            runflag = self.toolkit._makedir(self.work_dir)
            # if needed backup requirements of TestRelease
            # and then copy TestRelease package
            runinfo_list = self.toolkit._prepare_package(self.TestRelease, self.TestRelease_dir, dobackup = True)
            runflag = runinfo_list[0] # renew runflag
            if runflag:
                # configure work.bossversion and renew runflag
                runflag = self._config_TestRelease(runinfo_list)
                if runflag:
                    runflag =  self._make_bossbin() # renew runflag
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'Setting up BOSS {self.bossversion} succeeded.\n')
            print(TextColor.blue(f'Setting up BOSS {self.bossversion} succeeded.'))
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'Setting up BOSS {self.bossversion} failed.\n')
            print(TextColor.red(f'Setting up BOSS {self.bossversion} failed or aborted.'))
        return runflag

    def _initial_runflag(self):
        with open(self.logfile, 'a') as a:
            a.write('################################\n')
            a.write(datetime.now().strftime('%Y/%m/%d %H:%M:%S'))
            a.write('\n')
            a.write(f'WALL⋅E starts setting up BOSS {self.bossversion}\n')
    
    def _config_cmthome(self):
        runflag = False
        # enter cmthome-bossversion
        os.chdir(self.cmthome_dir)
        # modify requirements
        source_keywords = ['#macro WorkArea', '#set WorkArea']
        target_line     = 'set WorkArea \"' + self.work_dir + '\"\n'
        self.toolkit._replace_line('requirements', source_keywords, target_line) 
        self.toolkit._replace_keyword('requirements', '#path_remove CMTPATH' , 'path_remove CMTPATH')
        self.toolkit._replace_keyword('requirements', '#path_prepend CMTPATH', 'path_prepend CMTPATH')
    
        # modify SetupCVS.csh and SetupCVS.sh
        self.toolkit._replace_keyword('setupCVS.csh', 'maqm', self.user)
        self.toolkit._replace_keyword('setupCVS.sh' , 'maqm', self.user)
        # source setupCMT.csh
        # cmt config
        # generate setup.csh, cleanup.csh and Makefile
        # get time
        now = datetime.now().strftime('%Y%m%d%H%M%S')
        tcsh_file = f'config+{now}.csh'
        with open(tcsh_file, 'w') as w:
            w.write('#!/bin/tcsh -f\n')
            w.write('source setupCMT.csh\n')
            w.write('cmt config\n')
            w.write('source setup.csh\n')
            w.write('echo $CMTPATH > CMTPATH.txt\n')
            #w.write('echo $BesArea > BesArea.txt\n')
        os.system(f'chmod +x {tcsh_file}')
        os.system(f'hep_container exec {self.container} ./{tcsh_file}') 
        print('------------------------------------------')
        if os.path.isfile('CMTPATH.txt'):
            with open('CMTPATH.txt') as f:
                grep_contents = f.readlines()
            print(TextColor.red('WALL⋅E is checking CMTPATH...'))
            if self.work_dir in grep_contents[0]:
                print(TextColor.blue('CMTPATH is OK.'))
                os.remove(tcsh_file)
                os.remove('CMTPATH.txt')
                # modify setup.csh, setup.sh, cleanup.csh and cleanup.sh  
                self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
                self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
                self.toolkit._replace_keyword('setup.csh'  , 'cmt ', 'cmt -disable_warnings ')
                self.toolkit._replace_keyword('setup.sh'   , 'cmt ', 'cmt -disable_warnings ')
                runflag = True
            else:
                print(TextColor.red('CMTPATH is wrong!'))
        else:
            print(TextColor.red('CMTPATH is empty!'))
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'cmthome-{self.bossversion} is configured.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'something wrong with cmthome-{self.bossversion}.\n')
        return runflag
    
    def _config_TestRelease(self, runinfo_list_prepare_package):
        runflag = False
        # find requirements in TestRelease and enter into its directory
        TestRelease_dir = os.path.join(self.environments.workfs, 'work.' + self.bossversion, 'TestRelease') 
        file_info_list = self.toolkit._findfile(TestRelease_dir, 'requirements')
        os.chdir(file_info_list[1])
        self.toolkit._replace_keyword('cleanup.csh', self.Boss, self.work_dir)
        self.toolkit._replace_keyword('cleanup.sh' , self.Boss, self.work_dir)
        self.toolkit._replace_keyword('setup.csh'  , self.Boss, self.work_dir)
        self.toolkit._replace_keyword('setup.sh'   , self.Boss, self.work_dir)
        if runinfo_list_prepare_package[0]:
            if len(runinfo_list_prepare_package) > 1:
                if os.path.isfile(os.path.join(file_info_list[1], runinfo_list_prepare_package[1])):
                    shutil.copyfile(runinfo_list_prepare_package[1], 'requirements')
                    os.remove(runinfo_list_prepare_package[1])
                    print('WALL⋅E recovered requirements')
                else:
                    print(TextColor.red(f'WALL⋅E cannot find the backup of requirements!'))
                    print(TextColor.red(f'WALL⋅E suggests you contact with Dr. Zhou!'))
                    with open(self.logfile, 'a') as a:
                        a.write(f'WALL⋅E cannot find the backup of requirements!\n')
                        a.write(f'something wrong with work.{self.bossversion}/TestRelease.\n')
                    return runflag
            # cmt broadcost cmt config
            # cmt broadcost cmt make
            # get time
            now = datetime.now().strftime('%Y%m%d%H%M%S')
            tcsh_file = f'config+{now}.csh'
            tcsh_script1 = os.path.join(self.cmthome_dir, 'setupCMT.csh')
            tcsh_script2 = os.path.join(self.cmthome_dir, 'setup.csh')
            with open(tcsh_file, 'w') as w:
                w.write('#!/bin/tcsh -f\n')
                w.write(f'source {tcsh_script1}\n')
                w.write(f'source {tcsh_script2}\n')
                w.write('cmt broadcast cmt config\n')
                w.write('cmt broadcast cmt make\n')
                w.write('source setup.csh\n')
            os.system(f'chmod +x {tcsh_file}')
            os.system(f'hep_container exec {self.container} ./{tcsh_file}') 
            print('------------------------------------------')
            self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
            self.toolkit._replace_keyword('cleanup.csh', 'cmt ', 'cmt -disable_warnings ')
            self.toolkit._replace_keyword('setup.csh'  , 'cmt ', 'cmt -disable_warnings ')
            self.toolkit._replace_keyword('setup.sh'   , 'cmt ', 'cmt -disable_warnings ')
            os.remove(tcsh_file)
            runflag = True
            with open(self.logfile, 'a') as a:
                a.write(f'work.{self.bossversion}/TestRelease is configured.\n')
        return runflag

    def _make_bossbin(self):
        runflag = False
        print(TextColor.magenta(f'WALL⋅E tries to make boss{self.short_bossversion}...'))
        # mkdir bossbin in workfs
        self.toolkit._makedir(self.bossbin_dir)
        # cd $HOME/bin/bossbin
        os.chdir(self.bossbin_dir)
        # create boss$shortversion.csh
        sourcefile = os.path.join(self.iRobot_dir, 'runcoms', 'boss')
        targetfile = os.path.join(self.bossbin_dir, 'boss'+ self.short_bossversion)
        shutil.copyfile(sourcefile, targetfile)
        # modify boss$shortversion.csh
        self.toolkit._replace_keyword(targetfile, 'USERID', self.user)
        self.toolkit._replace_keyword(targetfile, 'BOSSVERSION', self.bossversion)
        os.system(f'chmod +x {targetfile}')
        print(TextColor.blue('------------------------------------------'))
        os.system(f'hep_container exec {self.container} {targetfile}') 
        print(TextColor.blue('------------------------------------------'))
        # modify .tcshrc
        tcshrcfile = os.path.join(self.home, '.tcshrc')
        keyword_file = os.path.join(self.bossbin_dir, 'boss'+ self.short_bossversion)
        keyword = f"alias boss{self.short_bossversion} 'source {keyword_file}'\n"
        self.toolkit._insert_keyword(tcshrcfile, keyword)
        runflag = True
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'{targetfile} is ready.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'something wrong with {targetfile}.\n')
        return runflag


if __name__ == '__main__':
    # install BOSS 7.0.3
    #WALLE_setupBOSS = SetupBOSS('7.0.3')
    #WALLE_setupBOSS = SetupBOSS('7.0.3')
    #WALLE_setupBOSS = SetupBOSS('6.6.4.p02')
    WALLE_setupBOSS = SetupBOSS('7.0.9')
    WALLE_setupBOSS.install()
