#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Creat Algorithm package for BESIII analysis
# Author: Dr. ZHOU Xiang
# Email : xiangzhou@whu.edu.cn
# Data  : 2022-03-11

import os
import shutil
import subprocess
import distutils.dir_util

from datetime import datetime

from environments import Environments
from textcolor    import TextColor
from toolkit      import Toolkit

class CreateAlgorithm:
    """A class to create algorithm package from other template packages."""
    def __init__(self, bossversion, algname):
        self.bossversion       = bossversion
        self.algname           = algname
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
        self.bossbin           = self.environments.my_bossbin
        self.Boss              = self.environments.Boss
        self.iRobot_dir        = self.environments.preferences.iRobot_dir
        self.algtemplate       = ''
        self.algtemplatever    = ''
        self.algtemplatebossver= ''
        self.templates         = {'1':['JpsiTo2C3gamAlg', '6.6.4','00-00-06'],
                                  '2':['JpsiTophietaAlg', '6.6.4','00-00-06'],
                                  '3':['psipTo2pi4gamAlg','6.6.4','00-00-03'],
                                  '4':['psipTo4pi3gamAlg','6.6.4','00-00-02'],
                                  '5':['psipTo4pi4gamAlg','6.6.4','00-00-02']}

    def create(self):
        # set runflag
        runflag = False
        # write log
        self._initial_runflag()
        # check algorithm name
        nameflag = self._check_postfix()
        # check algorithm directory existence
        dirflag  = self._check_algExistence() 
        # select algorithm template
        if nameflag and dirflag:
            temflag = self._select_template()
            if temflag:
                runflag = self._create_package()



    def _initial_runflag(self):
        with open(self.logfile, 'a') as a:
            a.write('################################\n')
            a.write(datetime.now().strftime('%Y/%m/%d %H:%M:%S'))
            a.write('\n')
            a.write(f'WALL⋅E starts creating {self.algname}\n')

    def _check_postfix(self):
        """check the algorithm postfix, which should be Alg."""
        runflag = False
        if self.algname[-3:] != 'Alg':
            prompt         = self.algname + '\'s postfix is not Alg.\nPlease write a proper algorithm name: '
            prompt_warning = 'Please write a proper algorithm name: '
            name = input(TextColor.red(prompt))
            while not (name[-3:] == 'Alg'):
                name = input(TextColor.red(prompt_warning))
            self.algname = name # use new name
            runflag = True
        else:
            runflag = True
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'Algorithm is named as {self.algname}.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'{self.algname} is incorrect.\n')
        return runflag

    def _check_algExistence(self):
        """check the existence of algorithm. If not, create a directory."""
        absAlg_dir = os.path.join(self.work_dir, self.algname)
        runflag = os.path.isdir(absAlg_dir)
        if runflag:
            print(TextColor.red(f'{self.algname} exists. Abort!'))
            with open(self.logfile, 'a') as a:
                a.write(f'{self.algname} directory exists. Abort!\n')
        else:
            print(TextColor.red(f'{self.algname} does not exist. Go ahead!'))
            with open(self.logfile, 'a') as a:
                a.write(f'{self.algname} does not exist. Go ahead!\n')
        return not runflag

    def _select_template(self):
        """Let user choose an algorithm template."""
        runflag = False
        prompt         = 'Which one is your preferred algorithm template?: '
        prompt_warning = 'Please select a proper template: '
        # print template candidates in screen
        print(TextColor.magenta('The algorithm templates are:'))
        for key, value in self.templates.items():
            print(TextColor.blue(f' {key}: {value[0]}'))
        SerialNo = input(TextColor.magenta(prompt))
        while not (SerialNo in self.templates.keys()):
            SerialNo = input(TextColor.red(prompt_warning))
        self.algtemplate        = self.templates[SerialNo][0]
        self.algtemplatebossver = self.templates[SerialNo][1]
        self.algtemplatever     = self.templates[SerialNo][2]
        print(TextColor.blue(f'Your preferred algorithm template is {self.algtemplate}.'))
        runflag = True
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'Algorithm template is {self.algtemplate}.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'You do not choose any template.\n')
        return runflag

    def _create_package(self):
        # set runflag
        runflag = False
        # enter into work.bossversion
        os.chdir(self.work_dir)
        print(self.work_dir)
        # get time
        now = datetime.now().strftime('%Y%m%d%H%M%S')
        # set tcsh script file name
        tcsh_file = f'command+{now}.csh'
        with open(tcsh_file, 'w') as w:
            w.write('#!/bin/tcsh -f\n')
            w.write(f'source {self.bossbin}\n')
            w.write(f'cmt create {self.algname} {self.algname}-00-00-01\n') 
        os.system(f'chmod +x {tcsh_file}')
        # cmt create algname algname-00-00-01
        status = os.system(f'hep_container exec {self.container} ./{tcsh_file}')
        if not status:
            os.remove(tcsh_file)
        else:
            print(TextColor.red(f'Creating {self.algname} failed!'))
            with open(self.logfile, 'a') as a:
                a.write(f'Creating {self.algname} failed!')
                a.write(f'Please check {tcsh_file} in {self.work_dir}')
            return runflag
        # enter into algorithm directory and make directories
        os.chdir(f'{self.algname}/{self.algname}-00-00-01')
        os.mkdir(self.algname)
        os.mkdir('share')
        os.mkdir('src/components')
        # copy files from algorithm template and replace with algname
        # copy requirements in cmt
        sourcefile = os.path.join(self.workfs, 'work' + '.' + self.algtemplatebossver, self.algtemplate, self.algtemplate + '-' + self.algtemplatever, 'cmt', 'requirements')
        targetfile = os.path.join(os.getcwd(), 'cmt', 'requirements')
        shutil.copyfile(sourcefile, targetfile)
        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])
        # copy files to algname
        sourcedir = os.path.join(self.workfs, 'work' + '.' + self.algtemplatebossver, self.algtemplate, self.algtemplate + '-' + self.algtemplatever, self.algtemplate)
        targetdir = os.path.join(os.getcwd(), self.algname)
        if os.path.exists(sourcedir):
            for root, dirs, files in os.walk(sourcedir):
                for file in files:
                    if file[-2:] == '.h':
                        sourcefile = os.path.join(root, file)
                        if file[:-2] == self.algtemplate[:-3]:
                            targetfile = os.path.join(targetdir, self.algname[:-3] + '.h')
                        else:
                            targetfile = os.path.join(targetdir, file)
                        shutil.copyfile(sourcefile, targetfile)
                        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])
        # copy files to src and its subdir
        sourcedir = os.path.join(self.workfs, 'work' + '.' + self.algtemplatebossver, self.algtemplate, self.algtemplate + '-' + self.algtemplatever, 'src')
        targetdir = os.path.join(os.getcwd(), 'src')
        if os.path.exists(sourcedir):
            for root, dirs, files in os.walk(sourcedir):
                for file in files:
                    sourcefile = os.path.join(root, file)
                    if file == self.algtemplate[:-3] + '.cxx':
                        targetfile = os.path.join(targetdir, self.algname[:-3] + '.cxx')
                        shutil.copyfile(sourcefile, targetfile)
                        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])
                    if file == self.algtemplate[:-3] + '_load.cxx':
                        targetfile = os.path.join(targetdir, 'components', self.algname[:-3] + '_load.cxx')
                        shutil.copyfile(sourcefile, targetfile)
                        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])
                    if file == self.algtemplate[:-3] + '_entries.cxx':
                        targetfile = os.path.join(targetdir, 'components', self.algname[:-3] + '_entries.cxx')
                        shutil.copyfile(sourcefile, targetfile)
                        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])
        # copy file to share
        sourcedir = os.path.join(self.workfs, 'work' + '.' + self.algtemplatebossver, self.algtemplate, self.algtemplate + '-' + self.algtemplatever, 'share')
        targetdir = os.path.join(os.getcwd(), 'share')
        if os.path.exists(sourcedir):
            for root, dirs, files in os.walk(sourcedir):
                for file in files:
                    sourcefile = os.path.join(root, file)
                    if file == 'jobOptions_' + self.algtemplate[:-3] + '.txt':
                        targetfile = os.path.join(targetdir, 'jobOptions_' + self.algname[:-3] + '.txt')
                        shutil.copyfile(sourcefile, targetfile)
                        self.toolkit._replace_keyword(targetfile, self.algtemplate[:-3], self.algname[:-3])

if __name__ == '__main__':
    WALLE_createAlgorithm = CreateAlgorithm('7.0.9', 'ttttAlg')
    WALLE_createAlgorithm.create()
