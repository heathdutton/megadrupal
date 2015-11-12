#!/bin/bash

type=$1
iteration=$2

for (( i=1; $i <= $iteration; i = $(($i+1)) ));
do
  bash perf_test.sh $type
done
