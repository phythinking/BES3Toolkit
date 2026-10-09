#!/bin/bash

param=$1

if [[ $param == 703 ]]; then
    dir=(chic1 3872 3810 3900 4009 4090 4180 4190 4200 4210 4220 4230 4237 4245 4246 4260 4270 4280 4310 4360 4390 4420 4470 4530 4575 4600)
elif [[ $param == 705 ]]; then
    dir=(4130 4160 4290 4315 4340 4380 4400 4440)
elif [[ $param == 706 ]]; then
   dir=(4610 4620 4640 4660 4680 4700)
elif [[ $param == 707 ]]; then
    dir=(4740 4750 4780 4840 4914 4946)
else
    echo "You need to input 703、705、706 or 707"
    exit 1
fi

for i in "${dir[@]}"
do
    cd $i
    #boss.condor -wt mid -n 2 sim_3phi_"%{ProcId}" dst_3phi_"%{ProcId}"
    boss.condor -wt mid sig_3phi_0
    #bash sub 
    cd ..
done

