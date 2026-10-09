#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import shutil
import subprocess
import distutils.dir_util

from datetime import datetime

from environments import Environments
from textcolor import TextColor

class Toolkit:
    """A class contains tools for iRobot."""
    def __init__(self, bossversion):
        """initial Toolkit."""
        self.bossversion = bossversion
        self.environments      = Environments(self.bossversion)
        self.logfile           = self.environments.logfile

    def _makedir(self, target_absdir):
        """ make a directory."""
        runflag = False
        # make target (absolute) directory
        try:
            print(TextColor.red(f'WALL⋅E try to make {target_absdir}'))
            os.mkdir(target_absdir)
            print(TextColor.blue(f'{target_absdir} is created.'))
            runflag = True
            with open(self.logfile, 'a') as a:
                a.write(f'{target_absdir} is created.\n')
        except FileExistsError:
            print(TextColor.blue(f'{target_absdir} is OK.'))
            runflag = True
            #with open(self.logfile, 'a') as a:
            #    a.write(f'{target_absdir} is OK.\n')
        if not runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'Something wrong for handling {target_absdir}.\n')
        return runflag

    def _findfile(self, current_dir, filename):
        """ find a file deeply in a directory."""
        runflag = False
        file_info_list = [runflag]
        for item in os.walk(current_dir):
            if filename in item[2]:
                runflag = True
                file_info_list = [runflag, item[0], filename]
        return file_info_list

    def _backupfile(self, file_info_list):
        """ backup a file with a time postfix. """
        runflag = False
        if file_info_list[0]:
            now = datetime.now().strftime('%Y%m%d%H%M%S')
            srcfile = os.path.join(file_info_list[1], file_info_list[2])
            targetfile = os.path.join(file_info_list[1], file_info_list[2] +'.' + now)
            shutil.copyfile(srcfile, targetfile)
            runflag = True
            return [runflag, file_info_list[2] +'.' + now]
        else:
            return [runflag]

    def _prepare_package(self, source_dir, target_dir, dobackup = False, backupfile = 'requirements'):
        """copy package or backup up a file."""
        runflag = False
        try:
            # copy package 
            shutil.copytree(source_dir, target_dir)
            with open(self.logfile, 'a') as a:
                a.write(f'{target_dir} is copied.\n')
            runflag = True
            return [runflag]
        except FileExistsError:
            backupname = ''
            if dobackup:
                print(TextColor.magenta(f'WALL⋅E tries to backup {backupfile}...'))
                file_info_list = self._findfile(target_dir, backupfile)
                if file_info_list[0]:
                    print(TextColor.blue(f'WALL⋅E found {backupfile}'))
                    backup_info = self._backupfile(file_info_list)
                    backupname = backup_info[1]
                    print(TextColor.blue(f'{backupfile} is backuped as {backupname}.'))
                else:
                    print(TextColor.red(f'WALL⋅E cannot find {backupfile}!'))
                    print(TextColor.blue(f'WALL⋅E go ahead...'))
            # copy package, overwrite mode 
            distutils.dir_util.copy_tree(source_dir, target_dir)
            print(TextColor.blue(f'{target_dir} is ready.'))
            with open(self.logfile, 'a') as a:
                a.write(f'{target_dir} is ready for configuration.\n')
            runflag = True
            return [runflag, backupname]

    def _insert_keyword(self, targetfile, keyword):
        """ appends a line to a file. """
        runflag = False
        if os.path.isfile(targetfile):
            print(TextColor.magenta(f'WALL⋅E tries to update {targetfile}...'))
            with open(targetfile) as f:
                targetfile_contents = f.readlines()
            with open(targetfile, 'a') as a:
                Iskeyword = False
                for line in targetfile_contents:
                    if keyword in line:
                        Iskeyword = True
                if Iskeyword:
                    print(TextColor.blue(f'{targetfile} is OK.'))
                else:
                    a.write(keyword)
                    print(TextColor.blue(f'{targetfile} is updated.'))
            runflag = True
        else:
            print(TextColor.red(f'{targetfile} lost in home. Abort!'))
            with open(self.logfile, 'a') as a:
                a.write(f'{targetfile} lost!\n')
        if runflag:
            with open(self.logfile, 'a') as a:
                a.write(f'{targetfile} is OK.\n')
        else:
            with open(self.logfile, 'a') as a:
                a.write(f'something wrong with {targetfile}.\n')
        return runflag

    def _replace_line(self, document, source_keywords, target_line, add = False):
        """replaces or appends a line with source_keywords in document to target_line"""
        runflag = False
        document_file = document
        temporary_file    = 'temp'
        # read lines of document
        print(TextColor.magenta(f'WALL⋅E is reading {document_file}...'))
        with open(document_file) as f:
            document_contents = f.readlines()
        # write lines of temp
        print(TextColor.magenta(f'WALL⋅E is modifying {document_file}...'))
        with open(temporary_file, 'w') as w:
            for line in document_contents:
                writeflag = False
                for keyword in source_keywords:
                    if keyword in line:
                        writeflag = True
                        runflag = True
                        break
                if writeflag:
                    if add:
                        w.write(line)
                        w.write(target_line)
                        print(TextColor.magenta(f'WALL⋅E added a line in {document_file}...'))
                    else:
                        w.write(target_line)
                        print(TextColor.magenta(f'WALL⋅E replaced a line in {document_file}...'))
                else:
                    w.write(line)
        # check temp
        if os.path.isfile(temporary_file):
            os.remove(document_file)
            os.rename(temporary_file, document_file)
            if os.path.isfile(document_file):
                print(TextColor.blue(f'{document_file} is ready.'))
        else:
            print(TextColor.red(f'Temporary file for new {document_file} lost. Abort!'))
        return runflag
    
    def _replace_keyword(self, document, source_keyword, target_keyword):
        """WALL⋅E replaces source_keyword in document to target_keyword"""
        document_file = document
        temporary_file    = 'temp'
        # read lines of document
        print(TextColor.magenta(f'WALL⋅E is reading {document_file}...'))
        with open(document_file) as f:
            document_contents = f.readlines()
        # write lines of temp
        print(TextColor.magenta(f'WALL⋅E is modifying {document_file}...'))
        with open(temporary_file, 'w') as w:
            for line in document_contents:
                if source_keyword in line :
                    w.write(line.replace(source_keyword, target_keyword))
                else:
                    w.write(line)
        # check temp
        if os.path.isfile(temporary_file):
            os.remove(document_file)
            os.rename(temporary_file, document_file)
        else:
            print(TextColor.red(f'Temporary file for new {document_file} lost. Abort!'))
        # chech document
        if os.path.isfile(document_file):
            print(TextColor.blue(f'{document_file} is ready.'))
        else:
            print(TextColor.red(f'Modified {document_file} lost. Abort!'))
    

