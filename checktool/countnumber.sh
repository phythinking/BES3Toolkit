#!bin/bash
total_number_total=0
good_track_sel_total=0
gamma_sel_total=0
pid_total=0
vtx_total=0
fit_mode1_4c_total=0
fit_mode2_4c_total=0
fit_mode3_4c_total=0

# 循环遍历所有匹配的文件
for file in inc_gamV_psip_2021_*.txt.bosslog; do
		  # 检查文件是否存在
		  if [ -e "$file" ]; then
					 # 从文件中提取计数值
					 total_number=$(grep 'total number:' "$file" | awk '{print $NF}')
					 good_track_sel=$(grep 'Good Track Sel.:' "$file" | awk '{print $NF}')
					 gamma_sel=$(grep 'Gamma Sel.:' "$file" | awk '{print $NF}')
					 pid=$(grep 'PID:' "$file" | awk '{print $NF}')
					 vtx=$(grep 'vtx:' "$file" | awk '{print $NF}')
					 fit_mode1_4c=$(grep '4C for mode1 Fit:' "$file" | awk '{print $NF}')
					 fit_mode2_4c=$(grep '4C for mode2 Fit:' "$file" | awk '{print $NF}')
					 fit_mode3_4c=$(grep '4C for mode3 Fit:' "$file" | awk '{print $NF}')

					 # 将提取的值添加到总计数中
					 total_number_total=$((total_number_total + total_number))
					 good_track_sel_total=$((good_track_sel_total + good_track_sel))
					 gamma_sel_total=$((gamma_sel_total + gamma_sel))
					 pid_total=$((pid_total + pid))
					 vtx_total=$((fit_4c_total + fit_4c))
					 fit_mode1_4c_total=$((fit_mode1_4c_total + fit_mode1_4c))
					 fit_mode2_4c_total=$((fit_mode2_4c_total + fit_mode2_4c))
					 fit_mode3_4c_total=$((fit_mode3_4c_total + fit_mode3_4c))
		  fi
done

# 打印总计数
echo "Total Number Total: $total_number_total"
echo "Good Track Sel. Total: $good_track_sel_total"
echo "Gamma Sel. Total: $gamma_sel_total"
echo "PID Total: $pid_total"
echo "vtx Total: $vtx_total"
echo "4C for mode1 Fit Total: $fit_mode1_4c_total"
echo "4C for mode2 Fit Total: $fit_mode2_4c_total"
echo "4C for mode3 Fit Total: $fit_mode3_4c_total"
echo "Total Number Total: $total_number_total" >> number.txt
echo "Good Track Sel. Total: $good_track_sel_total" >> number.txt
echo "Gamma Sel. Total: $gamma_sel_total" >> number.txt
echo "PID Total: $pid_total" >> number.txt
echo "vtx Total: $vtx_total" >> number.txt
echo "4C for mode1 Fit Total: $fit_mode1_4c_total" >> number.txt
echo "4C for mode2 Fit Total: $fit_mode2_4c_total" >> number.txt
echo "4C for mode3 Fit Total: $fit_mode3_4c_total" >> number.txt
