#! /bin/bash
###########################################################
#This file is used to submit BESIII jobs
#Author Yangjunhui Modified by Dr.ZhouX's scripts
#date: 2023.05.25

if [[ $# -eq 3 ]]; then
    jobName="$1"
    bossversion="$2"
    bossbinWD="$3"
else
    echo "please append corrected arguments"
    exit 0
fi
shortver=$(echo "$bossversion" | tr -d . )
#echo shortver = $shortver

Queue="besq"

if [[ -f "${jobName}.sh" ]]; then
    rm "${jobName}.sh"
fi

cat > "${jobName}.sh" <<EOF
#!/bin/bash
source "$bossbinWD/boss${shortver}"
boss.condor ${jobName}.txt
EOF

chmod +x "${jobName}.sh"
#echo "${jobName}.sh is prepared"
