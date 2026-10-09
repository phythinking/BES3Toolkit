for((i=1;i<=40;i++))
do
    cd MC_0$i/all
    rm *root
    echo clear_all MC_$i
    cd ..
done
