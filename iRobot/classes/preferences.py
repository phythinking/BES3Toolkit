#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os

class Preferences:
    """A class to set all preferences."""
    
    def __init__(self):
        self.work_fs         = 'workfs2'
        self.scratch_fs      = 'scratchfs'
        self.bes_fs          = 'besfs5'
        # /workfs/bes
        self.workfs_prefix    = os.path.join('/', self.work_fs, 'bes')
        # /workfs/bes/zhoux/BES3Toolkit/iRobot
        self.iRobot_dir = os.path.join('/', self.work_fs, 'bes', 'zhoux', 'BES3Toolkit', 'iRobot')
        # /scratchfs/bes
        self.scratchfs_prefix = os.path.join('/', self.scratch_fs, 'bes')
        # /besfs5/users
        self.besfs_prefix	  = os.path.join('/', self.bes_fs, 'users')
        # /cvmfs/bes3.ihep.ac.cn/bes3sw/cmthome
        self.cmthome_prefix	  = os.path.join('/', 'cvmfs', 'bes3.ihep.ac.cn', 'bes3sw', 'cmthome')
        # /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss
        self.Boss_prefix	  = os.path.join('/', 'cvmfs', 'bes3.ihep.ac.cn', 'bes3sw', 'Boss')
        self.redhat_release	  = {'Boron':'SL5', 'Carbon':'SL6', 'Core':'SL7'}
        self.container_status = {'Boron': True, 'Carbon': True, 'Core':False}
        self.BOSS_container   = {'6':'SL5', '7':'SL6'}

    def show_preferences(self):
        print('The preferences are:')
        print(self.iRobot_dir)
        print(self.workfs_prefix)
        print(self.scratchfs_prefix)
        print(self.besfs_prefix)
        print(self.cmthome_prefix)
        print(self.Boss_prefix)

if __name__ == '__main__':
    # show the preferences
    pre = Preferences()
    pre.show_preferences()
