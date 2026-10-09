#!/bin/bash
# Yangjunhui modified from Dr.Zhoux
# Date: 2023.5.18
if [ $RobotArgSwitch == 0 ]; then
    clear
    if [ $NAME == yangjunhui ]; then
        echo -e "Hello, ${heavyblue}my BOSS${end}, I'm jarvis"
    else
        echo -e "Hello ${blue}$NAME${end}, I'm jarvis"
        echo "How can I do for you?"
    fi
fi
LINUX="$(cat /etc/redhat-release)"
if [ $RobotArgSwitch == 0 ]; then
    echo -e "Your Linux distrution is ${bggreen}$LINUX${end}."
fi


