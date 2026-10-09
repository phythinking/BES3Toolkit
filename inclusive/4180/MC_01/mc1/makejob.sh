#Describe:Generate Job submit script, inclusive MC rec to root
#Useage:systematic script, call by another script
#the times of MC
mcnumber=$1
#the process, like qqbar or DsDs*
list=$2
#the data file directory
# ⚠️  公开版本：真实数据集路径与 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
# 输入数据根目录：请设置环境变量 DATA_ROOT，或修改下面的默认值。
DATA_ROOT=${DATA_ROOT:-/path/to/offline/data}
dir=$DATA_ROOT/4180/mc/$mcnumber/dst/$list

dataFiles=`/bin/ls $dir/*dst `
fileNumber=`/bin/ls $dir/*dst | wc -l`

# the rec file number, since the event is 50000 in one rec file
# you can't take bigger number
nDataFile=6

#the job name with the format $myjob$iJob
iJob=0
myjob=A-DsSTDs-4180-${list}-

#copy by xiang's script, it is a dangerous operation
#since it will change raw script
sed -i "s/D0D0/$list/g" job.foot
#how many rec we have used
n=0
for recFile in $dataFiles
do 
	let m=n+1
	if [ ! -e $recFile ]
	then
		echo "This data file $recFile doesn't exist!"
		exit 1
	else
		line="\"$recFile\""
		#the recfile you put into one script is not the ndatafile
		#like 0,1,2,3,4,5, you should put more
		if [[ $n%$nDataFile -eq 0 ]]
		then
			# when come to 0, 6, 12, we should make the header
			# and if the file exist, we should make the foot 
			# and sometime let the ijob ++
			if [ -e ${myjob}${iJob} ]
			then
				sed -e s/8000/"$iJob"/ job.foot >> ${myjob}${iJob}
				let iJob++
			fi
			touch  ${myjob}${iJob}
			sed -e "s/MCNUMBER/$mcnumber/g"  job.head >> ${myjob}${iJob}
		fi

		if [[ $m%$nDataFile -eq 0 || $m -eq $fileNumber ]]
		then
			echo ${line} >> ${myjob}${iJob}
			let n++
		else
			echo $line, >> ${myjob}${iJob}
			let n++
		fi
	fi
done

#for the last jobOption file
sed -e s/8000/"$iJob"/ ./job.foot >> ${myjob}${iJob}


exit 0
