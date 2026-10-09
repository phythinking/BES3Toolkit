#!/bin/zsh
rm -f *~
rm -f .*
rsync -avz /Users/Xiang/cosmos/iRobot/earth/ ihep:/workfs2/bes/zhoux/BES3Toolkit/iRobot
ssh -t ihep "/workfs2/bes/zhoux/BES3Toolkit/iRobot/WALLE.py"
#ssh -t ihep "/workfs2/bes/zhoux/BES3Toolkit/iRobot/initialize.py"
#ssh -t ihep "/workfs2/bes/zhoux/BES3Toolkit/iRobot/setupboss.py"
#ssh -t ihep "/workfs2/bes/zhoux/BES3Toolkit/iRobot/creatAlgorithm.py"
ssh -t ihep "/workfs2/bes/zhoux/BES3Toolkit/iRobot/compileAlgorithm.py"
ssh ihep "tail -10 /workfs2/bes/zhoux/iRobot.log"

