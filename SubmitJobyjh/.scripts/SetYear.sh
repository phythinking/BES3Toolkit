#!/bin/bash

if [ "$motherparticle" == "jpsi" ]; then
	echo -e "As I know, the data set of $motherparticle is collected in ${blue}2009${end}, ${blue}2012${end}, ${blue}2018${end} and ${blue}2019${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}2009/2012/2018/2019/Quit${end}]: ")" inputyear 
    while true; do
        if [ "$inputyear" == "2009" ]; then
            year=2009
            break
        elif [ "$inputyear" == "2012" ]; then
            year=2012
            break
        elif [ "$inputyear" == "2018" ]; then
            year=2018
            break
        elif [ "$inputyear" == "2019" ]; then
            year=2019
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            exit 1
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}2009/2012/2018a/2018b/2019/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi

if [ "$motherparticle" == "psip" ]; then
	echo -e "As I know, the data set of $motherparticle is collected in ${blue}2009${end}, ${blue}2012${end} and ${blue}2021${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}2009/2012/2021/Quit${end}]: ")" inputyear 
    while true; do
        if [ "$inputyear" == "2009" ]; then
            year=2009
            break
        elif [ "$inputyear" == "2012" ]; then
            year=2012
            break
        elif [ "$inputyear" == "2021" ]; then
            year=2021
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            break
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}2009/2012/2021/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi

if [ "$motherparticle" == "psipp" ]; then
	echo -e "As I know, the data set of $motherparticle is collected in ${blue}round03${end}, ${blue}round04${end} and ${blue}round15${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}03/04/15/Quit${end}]: ")" inputyear
    while true; do
        if [ "$inputyear" == "03" ]; then
            year=2009
            break
        elif [ "$inputyear" == "04" ]; then
            year=2012
            break
        elif [ "$inputyear" == "15" ]; then
            year=2018
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            break
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}03/04/15/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi

if [ "$motherparticle" == "cms3650" ]; then
	echo -e "As I know, the data set of $motherparticle is collected in ${blue}round02${end}, ${blue}round06${end} and ${blue}round15${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}02/06/15/Quit${end}]: ")" inputyear
    while true; do
        if [ "$inputyear" == "02" ]; then
            year=02
            break
        elif [ "$inputyear" == "06" ]; then
            year=06
            break
        elif [ "$inputyear" == "15" ]; then
            year=15
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            break
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}03/04/15/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi

if [ "$motherparticle" == "cms3682" ]; then
	echo -e "As I know, the data set of $motherparticle is collected only in ${blue}round15${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}15/Quit${end}]: ")" inputyear
    while true; do
        if [ "$inputyear" == "15" ]; then
            year=15
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            break
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}15/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi

if [ "$motherparticle" == "cms3080" ]; then
	echo -e "As I know, the data set of $motherparticle is collected in ${blue}roubd02/round05/round11/round12${end}."
	read -p "$(echo -e "Which year will you concern? [${blue}02/05/11/12/Quit${end}]: ")" inputyear
    while true; do
        if [ "$inputyear" == "02" ]; then
            year=02
            break
        elif [ "$inputyear" == "05" ]; then
            year=05
            break
        elif [ "$inputyear" == "11" ]; then
            year=11
            break
        elif [ "$inputyear" == "12" ]; then
            year=12
            break
        elif [ "$inputyear" == "Quit" ] || [ "$inputyear" == "quit" ] || [ "$inputyear" == "q" ] || [ "$inputyear" == "Q" ]; then
            echo -e "I will quit as you wish"
            break
        else
            read -p "$(echo -e "Cannot understand! Which year?[${blue}02/05/11/12/Quit${end}]: ")" inputyear
            continue
        fi
    done
fi
