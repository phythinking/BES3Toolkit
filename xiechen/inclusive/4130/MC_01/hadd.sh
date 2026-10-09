a0=("D0D0" "DDPI0" "DDPIp" "DDSTPI0" "DDSTPIp" "DpDm" "DsDs.newLS" "DsSTDs.newLS" "DST0D0" "DST0DST0" "DSTpDm" "DSTpDSTm" "ee" "eeNLO" "HCT" "mm" "tt" "qq" "RR1S" "RR2S" "RR3770" "TwoGam")
cd all
rm *.root
for((i=1;i<=22;i++))
do
    let z=$i-1
    cd ../mc$i
    rm A-DsSTDs-4130-${a0[z]}.root
    hadd A-DsSTDs-4130-${a0[z]}.root A*.root
    mv A-DsSTDs-4130-${a0[z]}.root ../all
done
cd ..
ls all/*root|wc -l
root -q -l sel.c
