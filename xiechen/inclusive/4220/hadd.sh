for((i=1;i<=40;i++))
do
	cd MC_0$i
	bash hadd.sh&
	cd ..
done
