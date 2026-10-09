for((i=1;i<=40;i++))
do
    cd MC_0$i
    bash reset.sh&
    echo "MC_0$i"
    cd ..
done
