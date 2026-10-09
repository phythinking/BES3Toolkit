#! /bin/bash
#start BOSS
bossbinWD=/afs/ihep.ac.cn/users/${firstLetter}/$NAME/bin/bossbin
#check the BOSS version
if [ -z "$BES_RELEASE" ]; then
    echo -e "${red}BOSS${end} is unavailable ... ${heavyred}Wall.E${end} will turn it on!"
    shortver=$(echo "$bossversion" | tr -d .)
    source ${bossbinWD}/boss${shortver}
else
    echo -e "${red}BOSS${end} is here. Somthing wrong!"
    exit 2
fi