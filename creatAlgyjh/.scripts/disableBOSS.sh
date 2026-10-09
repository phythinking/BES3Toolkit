#! /bin/bash
#turn off the BOSS
homeWD=$HOME
workfsWD=/workfs2/bes/$NAME
cmthomeWD=/workfs2/bes/$NAME/cmthome
source ${cmthomeWD}/cmthome-${BES_RELEASE}/cleanup.sh
#check the BOSS version
if [ -z "$BES_RELEASE" ]; then
    echo -e "${red}BOSS${end} has been turned off"
else
    echo -e "${red}BOSS.${BES_RELEASE}${end} is still here. Somthing wrong!"
    return 2
fi