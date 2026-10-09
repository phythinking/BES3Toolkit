#! /bin/bash
# submit jobs to server

echo "All job txt files are prepared."
if [ ! $answer ]; then
    read -p "Will you continue? [yes/no]:" yn
fi
while true; do
   if [[ ! $answer ]]; then
            echo $answer
    else
            yn = $answer
            if [ $answer != yes ] && [ $answer != no ]; then
                echo "The 3th parameter of jarvis neither yes or no! Check it! "
                exit 1
            fi
    fi
    if [ $yn == yes ]; then
        source subjob.sh
        cd $iRd
        exit 0
    elif [ $yn == no ]; then
        echo "Abort as you wish!"
        cd $iRd
        exit 1
    else 
         read -p "Can't Understand! Will you continue? [yes/no]:" yn
         continue
    fi
done