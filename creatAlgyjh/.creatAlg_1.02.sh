#!/bin/bash -f
# Author: Yangjunhui modified from Dr.ZhouXiang's script
# Version: 1.01
# Still have bugs with cp oldAlg's file, and it needs a boss check
# Date: 2023.5.19

RobotArgSwitch=0
if [ $# -eq 3 ]; then
    directory=$1
    algName=$2
    bossversion=$3
    newalgName=$algName
elif [ $# -eq 4 ]; then
    directory=$1
    algName=$2
    bossversion=$3
    if [ "$4" == "-silent" ]; then
        RobotArgSwitch=1
    else
    		AlgVersion=$4
    fi
	 newalgName=$algName
elif [ $# -eq 5 ]; then
    directory=$1
    algName=$2
    bossversion=$3
    AlgVersion=$4
    newalgName=$5
else
    echo "please append corrected arguments! For example, creatAlg /DirectoryFoldofAlg oldAlg 7.0.8 00-00-01 newAlg"
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
noalgName=$(echo $algName | sed 's/Alg$//')
nonewalgName=$(echo $newalgName | sed 's/Alg$//')

# set working directories
homeWD=$HOME
SCRIPTS=.scripts
TXTS=txts
workfsWD=/workfs2/bes/$NAME
scratchfsWD=/scratchfs/bes/$NAME
iRd=$PWD
creatAlgWD=/workfs2/bes/yangjunhui/BES3Toolkit/creatAlgyjh
setupBOSSWD=/afs/ihep.ac.cn/users/y/yangjunhui/setupBOSS

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

bossbinWD="/afs/ihep.ac.cn/users/${firstLetter}/$NAME/bin/bossbin"
cmthomeWD="/workfs2/bes/$NAME/cmthome"

if [[ ! -d /cvmfs/bes3.ihep.ac.cn/bes3sw/cmthome/cmthome-$bossversion && ! -d /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion ]]; then
    echo "BOSS $bossversion doesn't exist. Please check it!"
    return 1
fi

# set colors
source $setupBOSSWD/$SCRIPTS/SetColors.sh
if [ $? -ne 0 ]; then
    return 1
fi

# check the linux distribution
source $setupBOSSWD/$SCRIPTS/welcome.sh
if [ $? -ne 0 ]; then
    return 1
fi

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
                return 1
            fi
        fi
        if [[ $yn == yes ]]; then
            echo "Install BOSS $bossversion ..."
            $setupBOSSWD/installBOSS $bossversion
            break
        elif [[ $yn == no ]]; then
            echo "Get it! Good luck!"
            return 0
        else
            echo -n "Cannot understand! Will you intall BOSS $bossversion? [yes/no]:  "
            continue
        fi
    done
fi

if [ -d $workfsWD/work.$bossversion/${newalgName} ]; then
    echo "Find ${newalgName} under work.$bossversion. Please check!"
    return 1
fi

if [[ $AlgVersion ]]; then
    if [[ ! -d $directory/${algName}/${algName}-$AlgVersion ]]; then
        echo "Cannot find ${algName} with version $AlgVersion under $directory. Please check it!"
        return 1
    fi
else
    # Is there only 1 algorithm package?
    cd $directory/${algName}
    AlgNumber=$(ls -d ${algName}-* | wc -w)
    if [[ $AlgNumber -eq 0 ]]; then
        echo "Cannot find ${algName} under $directory/${algName}. Please check it!"
        return 1
    fi
    if [[ $AlgNumber -gt 1 ]]; then
        echo "Can find $AlgNumber (more than 1) ${algName} Directories under $directory/${algName}. Abort! "
        return 1
    fi
    AlgshortVersion=$(ls -d ${algName}-* | grep -o '[^-]*$')
    AlgVersion=00-00-$AlgshortVersion
    # AlgVersion=$(ls -d ${algName}Alg-* | sed -e "s/^${algName}Alg-//")
fi

AlgSubD=${algName}-$AlgVersion
AlgDirectory=$directory/${algName}/$AlgSubD

source $creatAlgWD/$SCRIPTS/checkBOSSVersion.sh
if [ $? -ne 0 ]; then
    return 1
fi

cd $workfsWD/work.$bossversion

echo "all prepare work has done"

# prepare directory

if [[ ! -d $workfsWD/work.$bossversion/Analysis ]]; then
    echo "Analysis doesn't exist. Now mkdir Analysis"
    mkdir Analysis
fi
namepkg=${newalgName}
cd $workfsWD/work.$bossversion/Analysis

cmt create $namepkg ${namepkg}-00-00-01


cd ${namepkg}/${namepkg}-00-00-01
mkdir $namepkg
cd src
mkdir components
cd ..

pwd
echo "*************************"
echo $AlgDirectory
# copy all files needed
cp $AlgDirectory/cmt/requirements cmt/requirements
cp $AlgDirectory/${algName}/*.h ${namepkg}/
rm ${namepkg}/${noalgName}.h
cp $AlgDirectory/${algName}/${noalgName}.h ${namepkg}/${nonewalgName}.h
cp $AlgDirectory/src/${noalgName}.cxx src/${nonewalgName}.cxx
cp $AlgDirectory/src/*.cxx src/
rm src/${noalgName}.cxx
cp $AlgDirectory/src/components/${noalgName}_load.cxx src/components/${nonewalgName}_load.cxx
cp $AlgDirectory/src/components/${noalgName}_entries.cxx src/components/${nonewalgName}_entries.cxx
mkdir share
cp $AlgDirectory/share/jobOptions_${noalgName}.txt share/jobOptions_${nonewalgName}.txt

# relace names in these files
if [ -f temp ]; then
	rm temp
fi
sed -e s/${noalgName}/${nonewalgName}/g cmt/requirements > temp
mv temp cmt/requirements
sed -e s/${noalgName}/${nonewalgName}/g ${namepkg}/${nonewalgName}.h > temp
mv temp ${namepkg}/${nonewalgName}.h
# sed -e s/${noalgName}/${nonewalgName}/g src/${nonewalgName}.cxx > temp
# mv temp src/${nonewalgName}.cxx
for file in src/*.cxx; do
    sed -e "s/${noalgName}/${nonewalgName}/g" "$file" > temp
    mv temp "$file"
done
sed -e s/${noalgName}/${nonewalgName}/g src/components/${nonewalgName}_load.cxx > temp
mv temp src/components/${nonewalgName}_load.cxx
sed -e s/${noalgName}/${nonewalgName}/g src/components/${nonewalgName}_entries.cxx > temp
mv temp src/components/${nonewalgName}_entries.cxx
sed -e s/${noalgName}/${nonewalgName}/g share/jobOptions_${nonewalgName}.txt > temp
mv temp share/jobOptions_${nonewalgName}.txt

# modify requirements in TestRelease
cd $workfsWD/work.$bossversion/TestRelease/*/cmt
sed -i '20i use JOBALG       JOBALG-*       Analysis' requirements
sed -e s/JOBALG/${namepkg}/g requirements > temp
mv temp requirements

cd $iRd

if [[ ! $answer ]]; then
    echo "$newalgName is prepared. Will you compile it? [yes/no]: "
fi
while true; do
    if [[ ! $answer ]]; then
        read -p "Enter your choice [yes/no]: " yn
    else
        if [[ $answer == yes || $answer == no ]]; then
            yn=$answer
        else
            echo "The 4th parameter of iRobot neither yes nor no. Please check it!"
            return 1
        fi
    fi
    if [[ $yn == yes ]]; then
        echo "Compile $newalgName ..."
        cd $workfsWD/work.$bossversion/Analysis/${namepkg}/${namepkg}-00-00-01/cmt
        cmt clean
        cmt config
        cmt make
        break
    elif [[ $yn == no ]]; then
        echo "Get it! Good luck!"
        return 0
    else
        echo -n "Cannot understand! Will you compile $newalgName? [yes/no]:  "
        continue
    fi
done

echo "$newalgName for BOSS $bossversion is ready!"

