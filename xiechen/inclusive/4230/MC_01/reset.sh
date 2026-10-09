a0=("D0D0" "DDPI0" "DDPIp" "DDSTPI0" "DDSTPIp" "DpDm" "DsDs.newLS" "DsSTDs.newLS" "DST0D0" "DST0DST0" "DSTpDm" "DSTpDSTm" "ee" "eeNLO" "HCT" "mm" "tt" "qq" "RR1S" "RR2S" "RR3770" "TwoGam")

rm mc1/A*
rm mc1/*.sub
rm sub.sh
for((i=2;i<=22;i++))
do
	rm mc$i -r
	cp mc1 mc$i -r
done

for((i=1;i<=22;i++))
do
	let z=$i-1
	cd mc${i}
	sed -i "s/mode.mctag = 1/mode.mctag = ${i}/g" job.head
	bash makejob.sh 01 ${a0[z]}

    declare -a Jobname
	for job in `/bin/ls A-*`
    do
    Jobname+=($job)
    done
	str1=$(basename $(pwd))
	str2=$(basename $(dirname $(pwd)))
	str3=$(basename $(dirname $(dirname $(pwd))))
	htc_parent_job="${str3}_""${str2}_""${str1}.sub"

	echo -e "#!/bin/bash

procid=\$1

job_scripts=(${Jobname[@]})

boss.exe \"\${job_scripts[\$procid]}\"
" > ${htc_parent_job}

    chmod 755 ${htc_parent_job}

    job_amount=${#Jobname[@]}

	cd ..

	if [ $job_amount -eq 1 ]
	then 
	echo -e "cd mc$i
boss.condor  A-DsSTDs-4230-${a0[z]}-0
cd ..
" >> sub.sh
	else
	echo -e "cd mc$i
boss.condor -n $job_amount A-DsSTDs-4230-${a0[z]}-\"%{ProcId}\"
cd ..
" >> sub.sh
fi

	unset Jobname
done
