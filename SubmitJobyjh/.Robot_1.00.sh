#!/bin/bash 
# interactive Robot (Shell Script) services for BOSS jobs
# Version:  1.00
# Author:   Yang Junhui modified from Dr.ZhouX's script 
# Date:     2023.5.23

if [ $# -eq 2 ]; then
    year=$1
    action=$2
    RobotArgSwitch=1
elif [ $# -eq 3 ]; then
    year=$1
    action=$2
    answer=$3
    RobotArgSwitch=1
elif [ $# -eq 4 ]; then
    year=$1
    action=$2
    answer=$3
    MCmodel=$4
    RobotArgSwitch=1
elif [ $# -eq 0 ]; then
    RobotArgSwitch=0
else
    echo "oops, something wrong, maybe you need append corrected arguments"
    exit 1
fi
# echo "test it**************************************"
# set short bossversion
shortver=$(echo "$bossversion" | tr -d .)
# get the USERID
NAME=$USER
# get the first letter of UserID
firstLetter=$(echo "$NAME" | cut -c1)
# set Now
NOW=$(date +"%Y%m%d%H%M%S")

# set scripts and txts directory
jarvisVersion=1.00
iRd=$PWD
SCRIPTS=.scripts
TXTS=txts
homeWD=$HOME
workfsWD=/workfs2/bes/$NAME
scratchfsWD=/scratchfs/bes/$NAME
creatAlgWD=/workfs2/bes/yangjunhui/BES3Toolkit/creatAlgyjh
setupBOSSWD=/afs/ihep.ac.cn/users/y/yangjunhui/setupBOSS
bossbinWD=/afs/ihep.ac.cn/users/${firstLetter}/$NAME/bin/bossbin
echo $bossbinWD
echo "****************************************"
cmthomeWD=/workfs2/bes/$NAME/cmthome


echo "basic set has down"

if [[ ! -d $SCRIPTS && ! -d $TXTS ]]; then
    echo "Can't find $SCRIPTS or $TXTS in Directory"
    if [ ! -l jarvis ]; then
        echo "Please run jarvis in its working directory!"
    fi
    exit 1
fi

# # check tcsh shell scripts in SCRIPTS directory
# if [ ! -e $SCRIPTS/checkScripts.sh ]; then
# 		  echo "jarvis can't self-check available scripts. Please put checkScripts.csh in $SCRIPTS directory. Abort!"
# 		  exit 1
# fi
# source $SCRIPTS/checkScripts.sh
# if [ $? ]; then
# 		  echo "Something's wrong in $SCRIPTS directory!"
# 		  exit 1
# fi

# Set Colors
source $SCRIPTS/SetColors.sh

# It will introduce itself, check the linux distribution, and then check whether BOSS is ready.
source $SCRIPTS/welcome.sh
if [ $? -ne 0 ]; then
    exit 1
fi

#set mother particle, i.e., the primary particle
source $TXTS/motherparticle.sh
if [ $? -ne 0 ]; then
    exit 1
fi
# echo "welcome*************************"

#set the year of dataset
if [ -z "$year" ]; then
    source $SCRIPTS/SetYear.sh
    if [ $? -ne 0 ]; then
    exit 1
    fi
fi
# echo "SetYear*************************"

#set the action
if [ -z "$action" ]; then
    source $SCRIPTS/SetAction.sh
    if [ $? -ne 0 ]; then
    exit 1
    fi
fi
# echo "SetAction*************************"

#determin the BOSS version and even RunIdList
source $SCRIPTS/SetBOSSVersion.sh
if [ $? -ne 0 ]; then
    exit 1
fi
# echo "BOSSVersion*************************"

#also check BOSS version
source $SCRIPTS/checkBOSSVersion.sh
if [ $? -ne 0 ]; then
    exit 1
fi
# echo "checkBOSS*************************"

# # compile analysis code if necessary
# source $SCRIPTS/compileAnalysisCode.sh
# if [ $? -ne 0 ]; then
#     exit 1
# fi

# # check parameters files in txt directory
# source $SCRIPTS/checkTxts.sh
# if [ $? -ne 0 ]; then
#     echo "Something's wrong in $TXTS directory. Please help Wall.E!"
#     exit 1
# fi

# set parameters
source $TXTS/parameters.sh
if [ $? -ne 0 ]; then
    exit 3
fi
# echo "parameters*************************"

# set datapath
source $TXTS/.datasetpath.sh
if [ $? -ne 0 ]; then
    exit 3
fi
# echo "datapath*************************"

# set jobNumber according to the action
source $SCRIPTS/SetjobNumber.sh
if [ $? -ne 0 ]; then
    exit 1
fi
# echo "jobNumber*************************"
# echo $jobNumber

# make dataset working directories(datasetWD), if exist then check jobs
source $SCRIPTS/checkOrMakeDirectories.sh
if [ $? -ne 0 ]; then
    exit 1
fi
# echo "checkDirectories*************************"

# enter action directory and prepare files and directories
cd $datasetWD/$jobDirectory
source $iRd/$SCRIPTS/PrepareFilesandDirectories.sh
if [ $? -ne 0 ]; then
    exit 1
fi
echo "prepare*************************"


# submit jobs to server
cd $datasetWD/$jobDirectory
source $iRd/$SCRIPTS/SubmitJobsToServer.sh
if [ $? -ne 0 ]; then
    exit 1
fi

