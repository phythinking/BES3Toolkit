#!/bin/bash
#Author: YangJunhui
#use this script to redistribute more tasks to the remaining .dst
# 新生成的 txt 文件的数量
num_copies=48

# 从 temp.txt 读取 dst 文件列表
dst_files=($(<temp.txt))

# 计算每个文件平均分配的 dst 数量
average_dst_per_file=$(((${#dst_files[@]} + $num_copies - 1) / $num_copies))

# 生成新的 txt 文件
for ((i = 1; i <= $num_copies; i++)); do
		  output_file="jobOptions_$((i-1)).txt"
  echo "// Include original files" > "$output_file"
  echo "#include \"\$ROOTIOROOT/share/jobOptions_ReadRec.txt\"" >> "$output_file"
  echo "#include \"\$VERTEXFITROOT/share/jobOptions_VertexDbSvc.txt\"" >> "$output_file"
  echo "#include \"\$MAGNETICFIELDROOT/share/MagneticField.txt\"" >> "$output_file"
  echo "#include \"\$ABSCORROOT/share/jobOptions_AbsCor.txt\"" >> "$output_file"
  echo "#include \"\$ETACTOGAMVALGROOT/share/jobOptions_etactogamV.txt\"" >> "$output_file"


  echo "" >> "$output_file"
  echo "// EventCnvSvc.digiRootInputFile" >> "$output_file"
  echo "EventCnvSvc.digiRootInputFile = {" >> "$output_file"

  for ((j = 1; j <= $average_dst_per_file; j++)); do
    index=$((i + (j - 1) * $num_copies))
    if [ $index -le ${#dst_files[@]} ]; then
      echo "\"${dst_files[index - 1]}\"," >> "$output_file"
    fi
  done
  sed -i '$ s/,$//' "$output_file"
  echo "};" >> "$output_file"

  echo "" >> "$output_file"
  echo "// Set output level threshold" >> "$output_file"
  echo "MessageSvc.OutputLevel = 6;" >> "$output_file"
  echo "ApplicationMgr.EvtMax = -1;" >> "$output_file"
  echo "ApplicationMgr.HistogramPersistency = \"ROOT\";" >> "$output_file"
  echo "NTupleSvc.Output = { \"FILE1 DATAFILE='root/data_gamV_jpsi_2012_417_${i}.root' OPT='NEW' TYP='ROOT'\" };" >> "$output_file"
done
