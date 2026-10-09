#!/bin/bash
#Author: Yang Junhui modified by ZhouX's scripts
#Date: 2023.5.25

here=$PWD

if [ -f temp ]; then
    rm temp
fi

if [ $motherparticle == jpsi ]; then
    ecms=3097
elif [ $motherparticle == psip ]; then
    ecms=3686
elif [ $motherparticle == psipp ]; then
    ecms=3770
elif [ $motherparticle == cms3650 ]; then
    ecms=3650
fi

if [ $action == sim ] || [ $action == all ]; then
    actionseed=10
elif [ $action == rec ]; then
    actionseed=20
elif [ $action == ana ]; then
    actionseed=30
elif [ $action == inc ]; then
    actionseed=40
elif [ $action == data ]; then
    actionseed=50
fi

if [ $year == 2009 ]; then 
    yearseed=2009
elif [ $year == 2009a ]; then 
    yearseed=20091
elif [ $year == 2009b ]; then 
    yearseed=20092
elif [ $year == 2012 ]; then 
    yearseed=2012
elif [ $year == 2010 ]; then 
    yearseed=2010
elif [ $year == 2018 ]; then 
    yearseed=2018
elif [ $year == 2019 ]; then 
    yearseed=2019
elif [ $year == 2021 ]; then 
    yearseed=2021
fi


SN=0
while [[ $SN -lt ${jobNumber} ]]; do
    randomseed=$((1002 + SN * 10 + ecms + actionseed + yearseed))

    if [ "${action}" == "all" ]; then
        for subaction in "sim" "rec" "ana"
        do

            cp "$here/$TXTS/${subaction}_${jobName}_${motherparticle}_${year}.txt" "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/AAAA/${RunIdList[0]}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/DECAY/decay$MCmodel/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/BBBB/${RunIdList[1]}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/CCCC/$year/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/DDDD/$EvtMax/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/1002/${randomseed}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/JOBNAME/${jobName}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp 
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/SIMDIR/sim.${MCmodel}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/RECDIR/rec.${MCmodel}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/ALGNAME/${algName}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/ALGROOTNAME/${algROOTName}/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.rtraw/${jobName}_${SN}.rtraw/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.dst/${jobName}_${SN}.dst/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.root/${jobName}_${SN}.root/g" "$here/${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${subaction}_${jobName}_${motherparticle}_${year}_${SN}.txt"
        done
    else
            cp "$here/$TXTS/${action}_${jobName}_${motherparticle}_${year}.txt" "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/AAAA/${RunIdList[0]}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/DECAY/decay$MCmodel/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/BBBB/${RunIdList[1]}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/CCCC/$year/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/DDDD/$EvtMax/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/1002/${randomseed}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/JOBNAME/${jobName}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp 
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/SIMDIR/sim.${MCmodel}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/RECDIR/rec.${MCmodel}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/ALGNAME/${algName}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/ALGROOTNAME/${algROOTName}/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.rtraw/${jobName}_${SN}.rtraw/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.dst/${jobName}_${SN}.dst/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"

            sed -e "s/${jobName}.root/${jobName}_${SN}.root/g" "$here/${action}_${jobName}_${motherparticle}_${year}_${SN}.txt" > temp
            mv temp "${action}_${jobName}_${motherparticle}_${year}_${SN}.txt"
    fi
    #./boss.sh "${action}_${jobName}_${motherparticle}_${year}_${SN}" "$bossversion" "$bossbinWD"
    echo -n "$SN "
    ((SN++))
done
echo

cd "$here"



