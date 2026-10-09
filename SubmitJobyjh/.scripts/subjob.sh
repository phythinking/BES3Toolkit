#!/bin/bash
#Author: Yang Junhui modified from ZX's script

here=$PWD
shortver=$(echo "$bossversion" | tr -d . )

rm -rf checked.txt

# make a check status file 
cat > checked.txt <<EOF
0
EOF

if [ "${action}" == "all" ]; then
    cat > sub.sh <<EOF
#!/bin/bash
boss.condor -n $jobNumber sim_${jobName}_${motherparticle}_${year}_"%{ProcId}".txt rec_${jobName}_${motherparticle}_${year}_"%{ProcId}".txt ana_${jobName}_${motherparticle}_${year}_"%{ProcId}".txt
EOF
else
    cat > sub.sh <<EOF
#!/bin/bash
boss.condor -n $jobNumber ${action}_${jobName}_${motherparticle}_${year}_"%{ProcId}".txt
EOF
fi

chmod +x sub.sh
source sub.sh
cd "$here"
