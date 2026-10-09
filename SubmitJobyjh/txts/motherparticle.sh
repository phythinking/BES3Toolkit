#!/bin/bash
#set the primary particle, i.e., jpsi, psip, ...
motherparticle=jpsi
if [ -z "$motherparticle" ]; then
		  echo "motherparticle is empty"
		  exit 1
fi
if [ $RobotArgSwitch -eq 0 ]; then
		  echo -e "The primary particle for your analysis is ${cyan}$motherparticle${end}"
fi
