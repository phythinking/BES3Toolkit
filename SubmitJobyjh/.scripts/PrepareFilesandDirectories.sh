#! /bin/bash
# Prepare Files and Directories

algROOTName=` echo $algName | tr "[a-z]" "[A-Z]" `
# copy txts to jobDirectory
cp $iRd/$SCRIPTS/boss.sh .
cp $iRd/$SCRIPTS/subjob.sh .
cp $iRd/$SCRIPTS/fill_all_failed_jobs.sh .
cp $iRd/$TXTS/parameters.sh .
cp /workfs2/bes/yangjunhui/BES3Toolkit/checktool/checkjoberr.sh .
sed -i "4i #boss$shortver" checkjoberr.sh
if [[ $action == sim || $action == rec || $action == ana || $action == all ]]; then
    mkdir $TXTS
    cp $iRd/$SCRIPTS/changen.sh .
fi
if [[ $action == rec || $action == ana ]]; then
    cp $iRd/$TXTS/.$action.txt $TXTS/${action}_${jobName}_${motherparticle}_${year}.txt
    chmod +w $TXTS/${action}_${jobName}_${motherparticle}_${year}.txt
fi
if [[ $action == sim ]]; then
    cp $iRd/$TXTS/.${action}_${motherparticle}.txt $TXTS/${action}_${jobName}_${motherparticle}_${year}.txt
    chmod +w $TXTS/${action}_${jobName}_${motherparticle}_${year}.txt
    mkdir rtraw
    cp $iRd/$TXTS/decay$MCmodel.dec .
    cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/Bes*/share/pdt.table .
    cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/Bes*/share/DECAY.DEC .
    # cp /scratchfs/bes/yangjunhui/fracchraged/txts/mypdt.table .
elif [[ $action == rec ]]; then
    mkdir dst
elif [[ $action == ana || $action == inc || $action == data ]]; then
    mkdir root
elif [[ $action == all ]]; then
    cp $iRd/$TXTS/.sim_${motherparticle}.txt $TXTS/sim_${jobName}_${motherparticle}_${year}.txt
    chmod +w $TXTS/sim_${jobName}_${motherparticle}_${year}.txt
    mkdir rtraw
    cp $iRd/$TXTS/decay$MCmodel.dec .
    cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/Bes*/share/pdt.table .
    cp /cvmfs/bes3.ihep.ac.cn/bes3sw/Boss/$bossversion/Generator/BesEvtGen/Bes*/share/DECAY.DEC .
    cp $iRd/$TXTS/.rec_all.txt $TXTS/rec_${jobName}_${motherparticle}_${year}.txt
    cp $iRd/$TXTS/.ana_all.txt $TXTS/ana_${jobName}_${motherparticle}_${year}.txt
    chmod +w $TXTS/rec_${jobName}_${motherparticle}_${year}.txt
    chmod +w $TXTS/ana_${jobName}_${motherparticle}_${year}.txt
    mkdir dst
    mkdir root
else
    echo "Wired! You should never see the massage! Pleack check iRobot!"
    exit 1
fi

echo "Generating job txts and shell scripts"
if [[ $action == sim || $action == rec || $action == ana || $action == all ]]; then
    source changen.sh 
elif [[ $action == inc || $action == data ]]; then
    cp $iRd/$SCRIPTS/makeJob.sh makeJob.sh
    cp $iRd/$TXTS/.makeJob.head makeJob.head
    chmod +w makeJob.head
    cp $iRd/$TXTS/.makeJob.tail makeJob.tail
    chmod +w makeJob.tail
    source makeJob.sh 
fi
if [[ $? -ne 0 ]]; then
    echo "Generating job failed. Check it!"
    exit 1
fi



