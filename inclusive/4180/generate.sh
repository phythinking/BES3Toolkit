sed -i "s/mode/$1/g" MC_01/mc1/job.head
sed -i "s/mode/$1/g" MC_01/reset.sh

for((i=2;i<=9;i++))
do
    rm MC_0$i -r
    cp MC_01 MC_0$i -r
    sed -i "s/01/0$i/g" MC_0$i/reset.sh
done
for((i=10;i<=40;i++))
do
    rm MC_0$i -r
    cp MC_01 MC_0$i -r
    sed -i "s/01/$i/g" MC_0$i/reset.sh
done
