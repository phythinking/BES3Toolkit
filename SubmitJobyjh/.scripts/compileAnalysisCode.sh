#!/bin/bash
# set compileAlg Directory
set compileAlgWD = /afs/ihep.ac.cn/users/z/zhoux/compileAlg
# set algorithm ROOT name. Don't modify!
algROOTName=` echo $algName | tr "[a-z]" "[A-Z]" `
if [ -z "$algROOTName" ]; then
    echo algROOTName is empty, check algName
    exit 1
fi

if [ "$action" = "ana" ] || [ "$action" = "inc" ] || [ "$action" = "data" ]; then
    # check analysis algorithm package existence
    AlgDBOSS=` eval echo \$${algROOTName}ALGROOT `
    if [ $? -ne 0 ]; then
        echo "Wall.E cannot find ${algName}Alg in BOSS $bossversion"
        echo "Don't panic. Wall.E will try to find it under work.$bossversion ..."
        $compileAlgWD/compileAlg ${algName} $bossversion 
        # after compile restart BOSS
        source $SCRIPTS/disableBOSS.sh
        source $SCRIPTS/enableBOSS.sh
        if [ $? -ne 0 ]; then
            echo "Compile Failed! Please help Wall.E!"
            exit 1
        fi
    fi
    # set algorithm version. Dont' modify!
    algVersion=` eval echo \$${algROOTName}ALGROOT | grep -o '[^/]*$' | grep -o '[^-]*$' | tr "[a-z]" "[A-Z]" `
    if [ -z "$algVersion" ]; then
        echo algVersion is empty, check algName
        exit 1
    fi
    if [ ! -d $workfsWD/work.$bossversion/Analysis/${algName}Alg/${algName}Alg-00-00-$algVersion ]; then
        echo "Analysis version is inconsistent with the one in BOSS"
        if [ -z "$answer" ]; then
        echo -n "Will you compile the algorithm? [yes/no]: "
        fi
        while true; do
            if [ "$?" -eq 0 ]; then
                read -p "Will you recover requirements file? [yes/no]: " yn
            else
                if [ "$answer" == "yes" ] || [ "$answer" == "no" ]; then
                    yn="$answer"
                else
                    echo "The 3th paramter of iRobot neither yes or no. Please check it!"
                    return 1
                fi
            fi
            if [ "$yn" == "yes" ]; then
                break
            elif [ "$yn" == "no" ]; then
                echo "Abort as you wish. Bye!"
                exit 1
            else
                read -p "Cannot understand! Will you compile? [yes/no]: " yn
                continue
            fi
        done
    fi
fi





