name=eeToKSKstar
dir=("4130" "4160" "4180" "4190" "4190-scan" "4200" "4210" "4210-scan" "4220" "4220-scan" "4230" "4230-scan")

# ⚠️  公开版本：真实数据集路径与 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
runbegin=("RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN" "RUN_BEGIN")

runend=("RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END" "RUN_END")

ecmenergy=("2.06424" "2.07872" "2.09" "2.094495" "2.094295" "2.099515" "2.104625" "2.103865" "2.10942" "2.108565" "2.11277" "2.11313")

for((i=0; i<12; i++))
do
    cd ${dir[$i]}
    ./generate.sh $name
    cd ..
done
