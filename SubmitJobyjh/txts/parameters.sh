#!/bin/bash -f
# parameters for iRobot

# Short name of jobs, e.g., phieta
jobName=lambdap

# Set the Number of events for sim/rec/ana per job. Normally, 100 for test or 10000 for real run
#EvtMax=100 # for testing
EvtMax=10000 # for real run

# Number of jobs for signal MC, inclusive MC and data, e.g., (50, 300, 300)
if [ $year == 2009 ]; then
	jobNumbers=(100 200 200)
elif [ $year == 2012 ]; then
	jobNumbers=(100 600 800)
elif [ $year == 2018 ]; then
	jobNumbers=(100 900 120)
elif [ $year == 2019 ]; then
	jobNumbers=(100 1200 1500)
elif [ $year == 2021 ]; then
	jobNumbers=(100 1000 1000)
elif [ $year == 02 ]; then
	jobNumbers=(100 1000 100)
elif [ $year == 06 ]; then
	jobNumbers=(100 1000 20)
elif [ $year == 15 ]; then
	jobNumbers=(100 1000 300)
fi


# Notice: the total event number of signal MC is EvtMax * jobNumbers[1]

# set signal MC model, e.g., HELAMP
if [ -z "$MCmodel" ]; then
#MCmodel=5CetaTopi0eePHSP
MCmodel=4CDIY
fi

# set algorithm name, e.g. JpsiTophieta which must consists with "use JpsiTophietaALg-* ..." in requirements of TestRelease/*/cmt
algName=PPKPi
if [ $algName == ""  ]; then
		  echo "algName cannot be  empty, check algName"
		  exit 1
fi

