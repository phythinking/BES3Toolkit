# !/bin/bash
# make dataset directory

if [ ! -d ${motherparticle}${year} ]; then
    mkdir ${motherparticle}${year}
fi

# set dataset working Directory
datasetWD=$iRd/${motherparticle}${year}

# enter the Working Directory
cd $datasetWD

# set jobDirectory
if [ "$action" == "sim" ] || [ "$action" == "rec" ]; then
    jobDirectory=${action}.${MCmodel}
elif [ "$action" == ana ] || [ "$action" == all ]; then
    jobDirectory=ana${algVersion}.${MCmodel}
elif [ "$action" == inc ] || [ "$action" == data ]; then
    jobDirectory=${action}${algVersion}
else 
    echo $action
fi

# creat job directory
if [ ! -d $jobDirectory ]; then
    echo "funny,**************************"
    NOW=$(date +"%Y%m%d%H%M%S")
    mkdir ${jobDirectory}.${NOW}
    rm -rf ${jobDirectory}
    ln -sd ${jobDirectory}.${NOW} ${jobDirectory}
else
    echo "$jobDirectory exists! Now I'll check it..."
    if [ -e $jobDirectory/checked.txt ]; then
        checked=`cat $jobDirectory/checked.txt`
        if [ $checked == 0 ]; then 
            echo "It seems that you have submitted jobs last time. Let me see ... "
            hep_q -u $NAME | grep -q "${action}_${jobName}_${motherparticle}_${year}"
            if [[ $? -eq 0 ]]; then
                echo "Jobs are running. Be patient! See you later!"
                exit 1
            else
                echo "Wall.E could't find any running jobs! But jobs need check."
                if [[ ! $answer ]]; then
                read -p "Will you check jobs? [yes/no]: " answer
                fi
            fi
            while true; do
                if [[ ! $answer ]]; then
                    read -p "Will you check jobs? [yes/no]: " yn
                else
                    yn=$answer
                    if [[ $answer != "yes" && $answer != "no" ]]; then
                        echo "The 3th paramter of iRobot neither yes or no. Please check it!"
                        exit 1
                    fi
                fi
                if [[ $yn == "yes" ]]; then
                    echo "Check jobs!"
                    cd "$datasetWD/$jobDirectory"
                    #copy fill_all_failed_jobs.csh again if it has been renewed
                    cp "$iRd/$SCRIPTS/fill_all_failed_jobs.sh" .
                    #./fill_all_failed_jobs.csh $action $jobName $jobNumber
                    source fill_all_failed_jobs.sh 
                    exit 1
                elif [[ $yn == "no" ]]; then
                    echo "Abort as you wish!"
                    exit 1
                else
                    read -p "Cannot understand! Will you continue? [yes/no]: " answer
                    continue
                fi
            done
        elif [ $checked == 1 ]; then
            echo "Congratulations! All jobs are successful! "
            exit 1
        else
        echo "It seems wired! Please check $jobDirectory directory carefully. Good luck!"
        exit 1
        fi
    else
        echo "It seems you tried but failed."
        while true
        do
            if [[ ! $answer ]]; then
                read -p "Will you do it again? [yes/no]: " yn
            else
                yn=$answer
                if [[ $answer != "yes" && $answer != "no" ]]; then
                    echo "The 3th paramter of iRobot neither yes or no. Please check it!"
                    exit 1
                fi
            fi
            if [[ $yn == "yes" ]]; then
                echo "For safety, backup ${jobDirectory}!"
                NOW=$(date +"%Y%m%d%H%M%S")
                mkdir "${jobDirectory}.${NOW}"
                rm -rf "$jobDirectory"
                ln -sd "${jobDirectory}.${NOW}" "$jobDirectory"
                break
            elif [[ $yn == "no" ]]; then
                echo "Abort as you wish!"
                exit 1
            else
                read -p "Cannot understand! Will you continue? [yes/no]: " answer
                continue
            fi
        done
    fi
fi







