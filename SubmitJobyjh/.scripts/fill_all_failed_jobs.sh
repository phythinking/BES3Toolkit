#!/bin/bash
here=$PWD

if [[ -e renamefilljobs.csh ]]; then
    if [[ -x renamefilljobs.csh ]]; then
        echo "Rename jobs."
        ./renamefilljobs.csh
    else
        rm renamefilljobs.csh
    fi
fi

cat > "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh" <<EOF
#!/bin/bash

# get procid from command line
procid=\$1

# map procid to sub_name if it exists
EOF

cat > renamefilljobs.sh <<EOF
#!/bin/bash
EOF

# set and initialize SN and badSN
SN=0
badSN=0

while [[ $SN -lt $jobNumber ]]; do
    if [[ -e tail.txt ]]; then
        rm tail.txt
    fi

    if [[ ! -e "${action}_${jobName}_${motherparticle}_${year}_$SN.txt" ]]; then
        echo "Weird! ${action}_${jobName}_${motherparticle}_${year}_$SN.txt does not exist! Abort!"
        exit 1
    elif [[ ! -e "${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog" ]]; then
        #echo "${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog does not exist"
        echo "if [[ \$procid -eq $badSN ]]; then" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
        echo "    sub_name=$SN" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
        echo "fi" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
        echo "" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
        echo "if [[ -e ${jobName}_${badSN}.bosslog ]]; then" >> renamefilljobs.sh
        echo "    mv ${jobName}_${badSN}.bosslog ${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog" >> renamefilljobs.sh
        echo "fi" >> renamefilljobs.sh
        echo "if [[ -e ${jobName}_${badSN}.bosserr ]]; then" >> renamefilljobs.sh
        echo "    mv ${jobName}_${badSN}.bosserr ${action}_${jobName}_${motherparticle}_${year}_$SN.bosserr" >> renamefilljobs.sh
        echo "fi" >> renamefilljobs.sh
        ((badSN++))
    else
        tail -3 "${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog" > tail.txt
        if [[ -z tail.txt ]]; then
            echo "${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog does not exist."
            echo "if [[ \$procid -eq $badSN ]]; then" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
            echo "    sub_name=$SN" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
            echo "fi" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
            echo "" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
            echo "mv ${jobName}_${badSN}.bosslog ${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog" >> renamefilljobs.sh
            echo "mv ${jobName}_${badSN}.bosserr ${action}_${jobName}_${motherparticle}_${year}_$SN.bosserr" >> renamefilljobs.sh
            ((badSN++))
        else 
            grep -q "INFO Application Manager successfully" tail.txt || grep -q "Application Manager Terminated successfully" tail.txt
            if [[ $? -ne 0 ]]; then
                echo "${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog shows that INFO Application Manager unsuccessfully!"
                echo "if [[ \$procid -eq $badSN ]]; then" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
                echo "    sub_name=$SN" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
                echo "fi" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
                echo "" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
                echo "mv ${jobName}_${badSN}.bosslog ${action}_${jobName}_${motherparticle}_${year}_$SN.bosslog" >> renamefilljobs.sh
                echo "mv ${jobName}_${badSN}.bosserr ${action}_${jobName}_${motherparticle}_${year}_$SN.bosserr" >> renamefilljobs.sh
                ((badSN++))
            fi
        fi
    fi
    echo -n "$SN "
    ((SN++))
    if [[ -e tail.txt ]]; then
        rm tail.txt
    fi
done

echo "# run the real job script by the sub_name " >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
echo "tcsh ${action}_${jobName}_${motherparticle}_${year}_\${sub_name}.sh" >> "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"

cd "$here"
if [[ $badSN -eq 0 ]]; then
    echo
    echo "No bad job! Congratulations! "
    rm "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
    rm renamefilljobs.sh
    sed -e 's/0/1/g' checked.txt > temp
    mv temp checked.txt
    exit 1
else
    echo "Find some bad jobs! Resubmit them!"
    chmod +x "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
    chmod +x renamefilljobs.sh
    if [[ ! -v answer ]]; then
        echo -n "Resubmit script is ready. Will you continue? [yes/no]: "
    fi
    while true; do
        if [[ ! -v answer ]]; then
            read -r yn
        else
            yn=$answer
            if [[ $answer != "yes" && $answer != "no" ]]; then
                echo "The 3th paramter of iRobot neither yes or no. Please check it!"
                exit 1
            fi
        fi
        if [[ $yn == "yes" ]]; then
            /afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/hep_sub "$here/${action}_${jobName}_${motherparticle}_${year}_resubjob.sh" -argu "%{ProcId}" -n $badSN -g physics -o ${jobName}_"%{ProcId}".bosslog -e ${jobName}_"%{ProcId}".bosserr
            cd "$here"
            exit 1
        elif [[ $yn == "no" ]]; then
            rm -rf renamefilljobs.sh
            rm -rf "${action}_${jobName}_${motherparticle}_${year}_resubjob.sh"
            echo "Abort as you wish!"
            cd "$here"
            exit 1
        else
            echo -n "Cannot understand! Will you continue? [yes/no]: "
            continue
        fi
    done
fi


