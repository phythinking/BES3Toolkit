#!/bin/bash
#set BOSS version according to the dataset(which has been specified by primary particle, dataset-year) and action
#
# ⚠️  公开版本：真实数据集路径与 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
#    请把下面每一处 RUN_BEGIN / RUN_END 换成你自己的 run 号区间。
#    例：RunIdList=(10000 20000)

if [ "$motherparticle" == "jpsi" ]; then
    bossversion=7.0.8
    if [ $year -eq 2009 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 2012 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 2018 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 2019 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ "$motherparticle" == "psip" ]; then
    bossversion=7.0.9
    if [ $year -eq 2009 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 2012 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 2021 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ "$motherparticle" == "cms3080" ]; then
    bossversion=7.0.8
    if [ $year -eq 02 ]; then
    #meaningless
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ "$motherparticle" == "cms3650" ]; then
    bossversion=7.0.9
    if [ $year -eq 02 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 06 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 15 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ "$motherparticle" == "cms3682" ]; then
    bossversion=7.0.9
    if [ $year -eq 15 ]; then
    #meaningless
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ "$motherparticle" == "psipp" ]; then
    bossversion=7.1.0
    if [ $year -eq 02 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 06 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
    if [ $year -eq 15 ]; then
        RunIdList=(RUN_BEGIN RUN_END)
    fi
fi

if [ $RobotArgSwitch -eq 0 ]; then
	echo -e "Then the ${red}BOSS${end} version should be ${red}$bossversion${end} ... "
fi

homeWD=$HOME
workfsWD=/workfs2/bes/$NAME
scratchfsWD=/scratchfs/bes/$NAME
creatAlgWD=/workfs2/bes/yangjunhui/BES3Toolkit/creatAlgyjh
setupBOSSWD=/afs/ihep.ac.cn/users/y/yangjunhui/setupBOSS

if [[ ! -d $workfsWD/cmthome/cmthome-$bossversion || ! -d $workfsWD/work.$bossversion ]]; then
    echo "BOSS $bossversion doesn't exist. Or BOSS $bossversion installation is not STANDARD:"
    echo "     cmthome-$bossversion under $workfsWD/cmthome"
    echo "     work.$bossversion    under $workfsWD"
    if [[ ! $answer ]]; then 
        echo "Don't panic. I can help you intall STANDARD BOSS $bossversion as you wish"
        echo -n "Will you intall BOSS $bossversion? [yes/no]: "
    fi
    while true; do
        if [[ ! $answer ]]; then
            read -p "Enter your choice [yes/no]: " yn
        else
            if [[ $answer == yes || $answer == no ]]; then
                yn=$answer
            else
                echo "The 4th parameter of iRobot neither yes nor no. Please check it!"
                exit 2
            fi
        fi
        if [[ $yn == yes ]]; then
            echo "Install BOSS $bossversion ..."
            $setupBOSSWD/installBOSS $bossversion
            break
        elif [[ $yn == no ]]; then
            echo "Get it! Good luck!"
            exit 0
        else
            echo -n "Cannot understand! Will you intall BOSS $bossversion? [yes/no]:  "
            continue
        fi
    done
fi

