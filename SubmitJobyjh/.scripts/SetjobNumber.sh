#!/bin/bash -f
# set jobNumber
if [ $action == "sim" ] || [ $action == "rec" ] || [ $action == "ana" ] || [ $action == "all" ]; then
		  jobNumber=${jobNumbers[0]}
elif [ $action == "inc" ]; then
		  jobNumber=${jobNumbers[1]}
elif [ $action == "data" ]; then
		  jobNumber=${jobNumbers[2]}
fi

