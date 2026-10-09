root=`ls m*/*root|wc -l`
log=`ls m*/*log|wc -l`
echo $root $log 
for((i=1;i<=22;i++))
do
    cd mc${i}
    bash error.sh
    cd ..
done
