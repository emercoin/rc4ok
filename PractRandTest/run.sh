#!/bin/sh
renice -n 10 $$
DATE=`date`
SEED="The_best_rc4ok_second__PractRand_test_8ff14b42bc30ddbded0fae8a780bdc9e43c51247"
echo "-------- $DATE --- $SEED ------" >> practrand_out.txt
./demo_rc4ok 1000000000 $SEED 0 | ./RNG_test stdin -multithreaded >>practrand_out.txt &
tail -f practrand_out.txt

