a0=("D0D0" "DDPI0" "DDPIp" "DDSTPI0" "DDSTPIp" "DpDm" "DsDs.newLS" "DsSTDs.newLS" "DST0D0" "DST0DST0" "DSTpDm" "DSTpDSTm" "ee" "eeNLO" "HCT" "mm" "tt" "qq" "RR1S" "RR2S" "RR3770" "TwoGam")
for((i=1;i<=22;i++))
do
	let z=$i-1
	cd mc$i
	rm A*.root 
	for bosslog in `/bin/ls *.bosslog`
	do
		echo "" > bosslog
	done
	for bosserr in `/bin/ls *.bosserr`
	do
		echo "" > bosserr
	done
	cd ..
done

