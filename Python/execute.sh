#!/usr/bin/env bash

for instance in 'p01' 'p02' 'p03' 'p04' 'p05' 'p06' 'p07' 'p08' 'p09' 'p10'
do 
    python3 src/main.py $instance -depth 3 -t 600
done
