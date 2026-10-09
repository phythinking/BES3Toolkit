#!/bin/bash

FIND_STR="INFO Application Manager Finalized successfully"
rm errorfile.txt
find . -maxdepth 1 -name "*.bosslog" -print0 | while read -d $'\0' file
# for file in `ls E*.bosslog`
do
    if [ `/bin/grep -c "$FIND_STR" $file` -eq 0 ]
    then
        echo -e "Not completed Job listed:\n"
        echo -e "${file%.*}\n"
        echo -e "${file%.*}\n" >> errorfile.txt
		#  boss.condor ${file%.*}
#    else
        # 获取软链接文件名
#        link_file="$(readlink "${file%.*}" | sed "s/\.[^.]*$//")"
	#echo $link_file
 #       cp "${file%.*}.bosslog" "$link_file.bosslog"
  #      cp "${file%.*}.bosserr" "$link_file.bosserr"
    fi
done

