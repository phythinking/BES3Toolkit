#Describe:search the failed Job and resubmit to HTcondor
#Useage:systematic script, call by another script
#!/bin/bash
FIND_STR="INFO Application Manager Finalized successfully"
declare -a Jobname
for file in `ls *.bosslog`
do
if [ `/bin/grep -c "$FIND_STR" $file` -eq 0 ]
then
    # one more time I use my favourite variable name ~~happy~~
    happy=${file%.*}
    Jobname+=($happy)
fi
done

str1=$(basename $(pwd))
str2=$(basename $(dirname $(pwd)))
str3=$(basename $(dirname $(dirname $(pwd))))
htc_parent_job="${str3}_""${str2}_""${str1}.err"

echo -e "#!/bin/bash 

procid=\$1

job_scripts=(${Jobname[@]})

boss.condor \"\${job_scripts[\$procid]}\"

" > ${htc_parent_job}

chmod 755 ${htc_parent_job}

job_amount=${#Jobname[@]}

if [ $job_amount -eq 0 ]
then 
echo "Every Job has been done!"
else
hep_sub -g physics -argu "%{ProcId}" -n $job_amount ${htc_parent_job} 
fi
