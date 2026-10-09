#!/bin/bash -f
# Author: Yangjunhui modified from Dr.ZhouXiang's script
# Version: 1.00
# This script will install setup BOSS

if [ $# -eq 1 ]; then
    bossversion=$1
    RobotArgSwitch=0
elif [ $# -eq 2 ]; then
    bossversion=$1
    if [ "$2" == "-silent" ]; then
        RobotArgSwitch=1
    fi
else
    echo "please append corrected arguments! For example, setupBOSS 6.6.4"
    return 1
fi

# set short bossversion
shortver=$(echo "$bossversion" | tr -d .)
# get the USERID
NAME=$USER
# get the first letter of UserID
firstLetter=$(echo "$NAME" | cut -c1)
# set Now
NOW=$(date +"%Y%m%d%H%M%S")
# set pwd
here=$PWD

# check directories
if [ ! -d "/workfs2/bes/$NAME" ]; then
    echo "Couldn't find workfs2 directory!"
    return 1
fi

if [ ! -d "/scratchfs/bes/$NAME" ]; then
    echo "Couldn't find scratchfs directory!"
    return 1
fi

if [ ! -d "/besfs5/users/$NAME" ]; then
    echo "Couldn't find besfs directory! Ignoring..."
fi

if [ "$HOME" != "/afs/ihep.ac.cn/users/${firstLetter}/$NAME" ]; then
    echo "home directory seems wired! Ignoring..."
fi

# set working directories
homeWD=$HOME
setupBOSSWD=/afs/ihep.ac.cn/users/y/yangjunhui/setupBOSS
SCRIPTS=.scripts
TXTS=txts
workfsWD=/workfs2/bes/$NAME
scratchfsWD=/scratchfs/bes/$NAME

# check if BOSS version exists
if [ ! -d "/cvmfs/bes3.ihep.ac.cn/bes3sw/cmthome/cmthome-$bossversion" ] || [ ! -d "/cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion" ]; then
    echo "BOSS $bossversion doesn't exist. Please check it!"
    return 1
fi


# set colors
source "$setupBOSSWD/$SCRIPTS/SetColors.sh"
if [ $? -ne 0 ]; then
    return 1
fi

# check the linux distribution, judge whether it runs in a container or not
source "$setupBOSSWD/$SCRIPTS/welcome.sh"
if [ $? -ne 0 ]; then
    return 1
fi

# set PATH
export PATH=/cvmfs/container.ihep.ac.cn/bin:$PATH
export PATH=/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin:$PATH

cd "$workfsWD"
if [ ! -d "cmthome" ]; then
    mkdir cmthome
fi
cd "$workfsWD/cmthome"
cp -rf "/cvmfs/bes3.ihep.ac.cn/bes3sw/cmthome/cmthome-$bossversion" .
cd $workfsWD/cmthome/cmthome-$bossversion

# modify requirements
sed -e 's;#macro WorkArea;set WorkArea;g' requirements > temp
mv temp requirements
sed -e 's;#set WorkArea;set WorkArea;g' requirements > temp
mv temp requirements
sed -e 's;/ihepbatch/bes/maqm/workarea;/workfs2/bes/USERID/work.BOSSVERSION;g' requirements > temp
mv temp requirements
sed -e 's;/home/bes/maqm/workarea;/workfs2/bes/USERID/work.BOSSVERSION;g' requirements > temp
mv temp requirements
sed -e 's;/ihepbatch/bes/maqm/cvmfs/;/workfs2/bes/USERID/work.BOSSVERSION;g' requirements > temp
mv temp requirements
sed -e 's;/home/bes/maqm/cvmfs/;/workfs2/bes/USERID/work.BOSSVERSION;g' requirements > temp
mv temp requirements
sed -e s/705//g requirements > temp
mv temp requirements
sed -e s/$shortver//g requirements > temp
mv temp requirements
sed -e s/USERID/$NAME/g requirements > temp
mv temp requirements
sed -e s/BOSSVERSION/$bossversion/g requirements > temp
mv temp requirements
sed -e s/"#path_remove CMTPATH"/"path_remove CMTPATH"/g requirements > temp
mv temp requirements
sed -e s/"#path_prepend CMTPATH"/"path_prepend CMTPATH"/g requirements > temp
mv temp requirements

# modify setupCVS.csh and setupCVS.sh
sed -e s/maqm/$NAME/g setupCVS.csh > temp
mv temp setupCVS.csh
sed -e s/maqm/$NAME/g setupCVS.sh > temp
mv temp setupCVS.sh

# source CMT scripts
source setupCMT.sh
cmt config

# modify setup.csh, setup.sh, cleanup.csh and cleanup.sh
sed -e s/"cmt "/"cmt -disable_warnings "/g cleanup.csh > temp
mv temp cleanup.csh
sed -e s/"cmt "/"cmt -disable_warnings "/g cleanup.sh > temp
mv temp cleanup.sh
sed -e s/"cmt "/"cmt -disable_warnings "/g setup.sh > temp
mv temp setup.sh
sed -e s/"cmt "/"cmt -disable_warnings "/g setup.csh > temp
mv temp setup.csh

source setup.sh
if [ $? -ne 0 ]; then
    echo "wwwwwwwwww"
    echo "CMTPATH is wrong!"
    return 1
fi

echo $CMTPATH > grep.txt
if [ -z "$grap.txt" ]; then
    echo "CMTPATH is wrong!"
    return 1
fi

grep -q "/workfs2/bes/$NAME/work.$bossversion" grep.txt;
if [ $? -ne 0 ]; then
    echo 2222
    echo "CMTPATH is wrong!"
    return 1
fi

#setup TestRelease
cd $workfsWD
if [ ! -d "work.$bossversion" ]; then
    mkdir "work.$bossversion"
fi

if [ -d "work.$bossversion/TestRelease" ]; then
    cd "$workfsWD/work.$bossversion/TestRelease"
    TestNumber=$(ls -d TestRelease-* | wc -w)
    if [ "$TestNumber" -eq 0 ]; then
        echo "There is no TestRelease subdirectory in work.$bossversion/TestRelease. Please check it!"
        return 1
    fi
    if [ "$TestNumber" -ne 1 ]; then
        echo "There is $TestNumber (more than 1) TestRelease subdirectory in work.$bossversion/TestRelease. Please check it!"
        return 1
    fi
    TestSubD=$(ls -d TestRelease-*)
    cd "$TestSubD"
    if [ -d "cmt" ]; then
        cd "cmt"
        if [ -e "requirements" ]; then
            echo "There is a requirements in TestRelease. Backup it!"
            cd $workfsWD/work.$bossversion/TestRelease/*/cmt
            cp requirements requirements_backup_$NOW
        fi
    fi
fi

cd "$workfsWD/work.$bossversion"
cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/BesEvtGen-*/share/pdt.table .
cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/BesEvtGen-*/share/DECAY.DEC .
cp -rf "$BesArea/TestRelease" .
cd $workfsWD/work.$bossversion/TestRelease/*/cmt


if [ -e "requirements_backup_$NOW" ]; then
    if [ "$?" -eq 0 ]; then
        echo -n "Will you recover requirements file? [yes/no]: "
    fi
    while true; do
        if [ "$?" -eq 0 ]; then
            read -p "Will you recover requirements file? [yes/no]: " yn
        else
            if [ "$answer" == "yes" ] || [ "$answer" == "no" ]; then
                yn="$answer"
            else
                echo "The 4th paramter of iRobot neither yes or no. Please check it!"
                return 1
            fi
        fi
        if [ "$yn" == "yes" ]; then
            mv "requirements_backup_$NOW" requirements
            break
        elif [ "$yn" == "no" ]; then
            echo "You can recover requirements_backup_$NOW later!"
            break
        else
            read -p "Cannot understand! Will you intall BOSS $bossversion? [yes/no]: " yn
            continue
        fi
    done
fi
#modify cleanup.csh
sed -e 's;-path=/cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/;-path=/workfs2/bes/USERID/work.;g' cleanup.csh > temp
mv temp cleanup.csh
sed -e s/USERID/$NAME/g cleanup.csh > temp
mv temp cleanup.csh

#modify cleanup.sh
sed -e 's;-path=/cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/;-path=/workfs2/bes/USERID/work.;g' cleanup.sh > temp
mv temp cleanup.sh
sed -e s/USERID/$NAME/g cleanup.sh > temp
mv temp cleanup.sh

#modify setup.csh
sed -e 's;-path=/cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/;-path=/workfs2/bes/USERID/work.;g' setup.csh > temp
mv temp setup.csh
sed -e s/USERID/$NAME/g setup.csh > temp
mv temp setup.csh

#modify setup.sh
sed -e 's;-path=/cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/;-path=/workfs2/bes/USERID/work.;g' setup.sh > temp
mv temp setup.sh
sed -e s/USERID/$NAME/g setup.sh > temp
mv temp setup.sh

cmt broadcast cmt config
if [ $? -ne 0 ]; then
    return 2
fi
cmt broadcast cmt make
if [ $? -ne 0 ]; then
    return 2
fi

#modify cleanup.csh and cleanup.sh
sed -e s/"cmt "/"cmt -disable_warnings "/g cleanup.csh > temp
mv temp cleanup.csh
sed -e s/"cmt "/"cmt -disable_warnings "/g cleanup.sh > temp
mv temp cleanup.sh
#modify setup.csh again
sed -e s/"cmt "/"cmt -disable_warnings "/g setup.csh > temp
mv temp setup.csh
#modify setup.sh again
sed -e s/"cmt "/"cmt -disable_warnings "/g setup.sh > temp
mv temp setup.sh

source setup.sh
if [ $? -ne 0 ]; then
    return 1
fi

#modify .bashrc
if [ $NAME == yangjunhui ]; then
    cd $HOME
    cp .bashrc .bashrc_backup_$NOW
    grep -q "alias boss$shortver" .bashrc
    if [ $? -ne 0 ]; then
        echo "alias boss$shortver='source /afs/ihep.ac.cn/users/${firstLetter}/$NAME/bin/bossbin/boss$shortver'" >> .bashrc
    fi
fi

if [ ! -d "$HOME/bin/bossbin" ]; then
    mkdir -p $HOME/bin/bossbin
fi
cd $HOME/bin/bossbin
cp -rf  $setupBOSSWD/$TXTS/boss boss$shortver
sed -e s/USERID/$NAME/g boss$shortver > temp
mv temp boss$shortver
sed -e s/BOSSVERSION/$bossversion/g boss$shortver > temp
mv temp boss$shortver
chmod +x boss$shortver


# set and check BOSS
cd $homeWD
if [ $NAME == yangjunhui ]; then
    source .bashrc
fi
# $HOME/bin/bossbin/boss$shortver
if [ $? -ne 0 ]; then
    return 3
fi

cd $here
echo "BOSS $bossversion is OK! Please enjoy it!"
return 0
