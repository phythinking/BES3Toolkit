#! /bin/bash
#check whether BOSS is suitable

#check the BOSS version
if [ -z "$BES_RELEASE" ]; then
   source $SCRIPTS/enableBOSS.sh
else 
    if [ "$bossversion" = "$BES_RELEASE" ]; then
        echo -e "Current ${red}BOSS${end} version is ${bgred}$BES_RELEASE${end}."
        echo -e "${red}BOSS${end} version matchs!"
    else 
        echo -e "${red}BOSS${end} version dismatchs! ${heavyred}Wall.E${end} will turn it off firstly!"
        source $SCRIPTS/disableBOSS.sh
        source $SCRIPTS/enableBOSS.sh
        if [ "$bossversion" = "$BES_RELEASE" ]; then
            echo -e "Current ${red}BOSS${end} version is ${bgred}$BES_RELEASE${end}."
            echo -e "${red}BOSS${end} version matchs!" 
        elif [ "$bossversion" != "$BES_RELEASE" ]; then
            echo -e "Weired! ${red}BOSS${end} version conflicts! ${heavyred}Wall.E${end} can't handle it!"
            exit 2
        fi
    fi
fi

