#!/bin/sh
renice -n 10 $$
DATE=`date`
# SEED="The_best_rc4ok_second__PractRand_test"
SEED="0"
echo "-------- $DATE --- $SEED ------" >> practrand_out.txt
./demo_rc4ok 1000000000 $SEED 0 | ./RNG_test stdin >>practrand_out.txt &
tail -f practrand_out.txt
