#!/bin/bash
# Yangjunhui modified from Dr.Zhoux
# Date: 2023.5.18
# latest modified: 2023.5.23
if [ $RobotArgSwitch -eq 0 ]; then
#    clear
    if [ $NAME == yangjunhui ]; then
        echo -e "Hello, ${heavyblue}my BOSS${end}, I'm jarvis"
    else
        echo -e "Hello ${blue}$NAME${end}, I'm jarvis"
        echo "How can I do for you?"
    fi
fi
LINUX="$(cat /etc/redhat-release)"
if [ $RobotArgSwitch -eq 0 ]; then
    echo -e "Your Linux distrution is ${bggreen}$LINUX${end}."
fi

if [ -z "$BES_RELEASE" ]; then
    if [ $RobotArgSwitch -eq 0 ]; then
        echo "Aha! BOSS is not here."
    fi
else
    if [ $RobotArgSwitch -eq 0 ]; then
        echo "Ooops! My BOSS is $BES_RELEASE"
    fi
fi

