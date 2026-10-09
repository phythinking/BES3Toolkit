#!/bin/bash -f
#Author: Yang Junhui modified from Dr.ZhouX's scripts
#Date: 2023.5.25
#The shell command generate jobs for inclusive MC and data

#!/bin/bash

# Assign the value of $jobNumber to $jobN
jobN=$jobNumber

echo "The number of jobs : ${jobN}"

# Assign the current working directory to $jobDir
jobDir=$(pwd)

# Find all .dst files in the specified directory and save the results to temp.txt
echo "DataParentPath : ${DataParentPath}"
if [ -e "${jobDir}/temp.txt" ]; then
    rm "${jobDir}/temp.txt"
fi

if [ ! -e "temp.txt" ]; then
    find "${DataParentPath}/" -name "*.dst" > temp.txt
else
    find "${DataParentPath}/" -name "*.dst" >> temp.txt
fi

# Count the number of lines in temp.txt and save the result to $nline
nline=$(wc -l < temp.txt)
echo "There are ${nline} rec file totally"

# Calculate the number of files in each job
fileNN=$((nline / jobN))
if [ $fileNN -eq 0 ]; then
    echo "The number of jobs ${jobN} is less then the one of rec files ${nline}. Abort!"
    exit 1
fi

# Calculate the number of extra files
extraOne=$((nline % jobN))
fileNNAddOne=$((fileNN + 1))
remainderNumber=$((jobN - extraOne))
echo "There will be ${fileNNAddOne} files in the first ${extraOne} jobs, and then ${fileNN} files in the remainder ${remainderNumber} jobs.."

# Make jobOptions with certain rec files in each job
iline=1
i=0
j=0
ijob=0
while read -r file; do
    if [ $j -lt $extraOne ]; then
        fileN=$((fileNN + 1))
    else
        fileN=$fileNN
    fi
    if [ $i -eq 0 ]; then
        if [ -e "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt" ]; then
            echo "Just remind: ${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt already exists, and it will be update!"
        fi
        # A trick: update non-verbosely
        sed -e "s/LLL/${action}_${jobName}_${motherparticle}_${year}_${ijob}/g" "${jobDir}/makeJob.head" > "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
    fi
    if [ $i -lt $fileN ]; then
        if [ $i -ne $((fileN - 1)) ]; then
            echo "\"${file}\"," >> "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
        else
            echo "\"${file}\"" >> "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
        fi
        i=$((i + 1))
    fi
    if [ $i -eq $fileN ]; then
        sed -e "s/XXX/${action}_${jobName}_${motherparticle}_${year}_${ijob}/g" "${jobDir}/makeJob.tail" >> "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
        sed -e "s/ALGNAME/${algName}/g" "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt" > temp
        mv temp "${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
        sed -e "s/ALGROOTNAME/${algROOTName}/g" "${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt" > temp
        mv temp "${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt"
        #echo ${jobDir}/${action}_${jobName}_${motherparticle}_${year}_${ijob}.txt is finished 
        #./boss.sh "${action}_${jobName}_${motherparticle}_${year}_${ijob}" "$bossversion" "$bossbinWD"
        i=0
        j=$((j + 1))
        echo -n "$ijob "
        ijob=$((ijob + 1))
    fi
    iline=$((iline + 1))
done < temp.txt
