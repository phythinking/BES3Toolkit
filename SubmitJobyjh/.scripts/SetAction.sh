#!/bin/bash
# set action 
# set job directory and judge action
read -p "$(echo -e "How can I help you? [${blue}sim/rec/ana/all/inc/data/Quit${end}]: ")" action

while true; do
    if [ "$action" == "sim" ]; then
    	echo -e "It seems that you want to submit signal MC ${red}simulation${end} jobs. "
		break
    elif [ "$action" == "rec" ]; then
    	echo -e "It seems that you want to submit signal MC ${red}reconstruction${end} jobs. "
		break
    elif [ "$action" == "ana" ]; then
    	echo -e "It seems that you want to submit signal MC ${red}analysis${end} jobs. "
		break
	elif [ "$action" == "all" ]; then
    	echo -e "It seems that you want to submit signal MC ${red}all${end} jobs. "
		break
    elif [ "$action" == "inc" ]; then
    	echo -e "It seems that you want to submit ${red}inclusive${end} MC analysis jobs. "
		break
    elif [ "$action" == "data" ]; then
    	echo -e "It seems that you want to submit ${red}data${end} analysis jobs. "
		break
    elif [ "$action" == "Quit" ] || [ "$action" == "quit" ] || [ "$action" == "q" ] || [ "$action" == "Q" ]; then
    	echo -e " See youuuu~ "
		exit 1
    else 
    	read -p "$(echo -e "Cannot understand! Which action?[${blue}sim/rec/ana/inc/data/Quit${end}]: ")" action
	    continue
    fi
done