#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os

from preferences import Preferences
from textcolor import TextColor

class Environments:
    """A class to get all environment variables."""

    def __init__(self, bossversion):
        """Detect the environments for the plant where WALL⋅E lives""" 
        self.bossversion = bossversion
        self.firstletter_bossversion = self.bossversion.split('.')[0]
        self.short_bossversion = ''.join(self.bossversion.split('.'))
        self.user      = os.environ['USER']
        self.home      = os.environ['HOME']
        self.shellname = os.environ['SHELL'].split('/')[-1]# '/bin/tcsh' -> 'tcsh'
        # set user's directories
        self.preferences    = Preferences()
        
        self.workfs    = os.path.join(self.preferences.workfs_prefix, self.user)
        self.scratchfs = os.path.join(self.preferences.scratchfs_prefix, self.user)
        self.besfs	   = os.path.join(self.preferences.besfs_prefix, self.user)
        self.dictionary_of_user_directories = {
        							   'workfs' : self.workfs, 
        							'scratchfs' : self.scratchfs, 
        							    'besfs' : self.besfs, 
        							}
        
        self.cmthome     = os.path.join(self.preferences.cmthome_prefix, 'cmthome-' + bossversion)
        self.Boss	     = os.path.join(self.preferences.Boss_prefix, bossversion)
        self.TestRelease = os.path.join(self.Boss, 'TestRelease')
        self.dictionary_of_BOSS_directories = {
        							  'cmthome' : self.cmthome, 
        							    'Boss'  : self.Boss,
        						   'TestRelease': self.TestRelease
        							}
        self.SLnickname  = Environments.SLnickname(self)
        self.SL          = Environments.checkSLstatus(self) 
        self.container   = Environments.whichContainer(self)
        self.logfile     = os.path.join(self.workfs, 'iRobot.log')
        self.cmthome_dir = os.path.join(self.workfs, 'cmthome', 'cmthome-' + self.bossversion)
        self.work_dir    = os.path.join(self.workfs, 'work.' + self.bossversion)
        self.TestRelease_dir = os.path.join(self.work_dir, 'TestRelease')
        self.my_dictionary_of_BOSS_directories = {
        							  'cmthome' : self.cmthome_dir, 
        							    'work'  : self.work_dir,
        						   'TestRelease': self.TestRelease_dir
        							}
        self.bossbin_dir = os.path.join(self.workfs, 'bossbin')
        self.my_bossbin	 = os.path.join(self.bossbin_dir, 'boss' + self.short_bossversion)
        self.my_files_of_BOSS = {
                'bossbin' : self.my_bossbin 
                }

    def _check_Directories(self, dictionary):
        runflag = False
        for name, dire in dictionary.items():
            if os.path.isdir(dire):
                runflag = True
                print(TextColor.blue(f"Check {name}... passed!"))
            else:
                print(TextColor.red(f"Check {name}... failed!"))
        return runflag

    def _check_files(self, filepaths):
        runflag = False
        for name, file in filepaths.items():
            if os.path.isfile(file):
                runflag = True
                print(TextColor.blue(f"Check {name}... passed!"))
            else:
                print(TextColor.red(f"Check {name}... failed!"))
        return runflag

    def checkBOSSDirectories(self):
        # check existence of BOSS in offline system
        print(TextColor.magenta(f"Check BOSS {self.bossversion} directories..."))
        runflag = self._check_Directories(self.dictionary_of_BOSS_directories)
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.bossversion} directories exist in offline system.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.bossversion} directories do not exist in offline system.\n')
        return runflag

    def checkMyBOSSstatus(self):
        # check existence of BOSS in workfs
        print(TextColor.magenta(f"Check BOSS {self.bossversion} directories in workfs..."))
        runflag = self._check_Directories(self.my_dictionary_of_BOSS_directories)
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.bossversion} directories exist in workfs.\n')
            runflag = self._check_files(self.my_files_of_BOSS)
            if runflag:
                with open(self.logfile, 'a') as a:
                    a.write(f'{self.bossversion} bin file exists in workfs.\n')
            else:
                with open(self.logfile, 'a') as a:
                    a.write(f'{self.bossversion} bin file does not exist in workfs.\n')

        else:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.bossversion} directories do not exist in workfs.\n')
        return runflag

    def checkUserDirectories(self):
        if self.user == 'zhoux':
            print(TextColor.magenta("Check directories of Dr. Zhou"))
        else:
            print(TextColor.magenta(f"Check directories of {self.user}"))
        runflag = self._check_Directories(self.dictionary_of_user_directories)
        return runflag

    def SLnickname(self):
        releasefile = '/etc/redhat-release'
        with open(releasefile) as f:
            release_content = f.readlines()
            nickname = ''.join(list(release_content[0].split()[-1])[1:-1])
        return nickname

    def checkSLstatus(self):
        return self.preferences.redhat_release[self.SLnickname]

    def InContainer(self):
        return self.preferences.container_status[self.SLnickname]

    def whichContainer(self):
        return self.preferences.BOSS_container[self.firstletter_bossversion]

    def selfcheck(self):
        print("workfs\t\t directory is"     , self.workfs)
        print("scratchfs\t directory is"    , self.scratchfs)
        print("besfs\t\t directory is"      , self.besfs)
        print("cmthome\t\t directory is"    , self.cmthome)
        print("Boss\t\t directory is"       , self.Boss)
        if self.InContainer():
            print(TextColor.blue(f"WALL⋅E is in {self.SL}, {self.SLnickname}, which is a container."))
        else:
            print(TextColor.blue(f"WALL⋅E is in {self.SL}, {self.SLnickname}, which is not a container."))
        print(TextColor.blue(f'BOSS {bossversion} should run in {self.container}.'))
        self.checkBOSSDirectories()
        self.checkUserDirectories()
        self.checkMyBOSSstatus()

if __name__ == '__main__':
    # show the environments of BOSS, default version is 7.0.5
    bossversion = input('What is your preferred BOSS version? (Default is 7.0.5): ')
    if bossversion == '':
        bossversion = '7.0.5'
    envir = Environments(bossversion)
    envir.selfcheck()

