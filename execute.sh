#!/usr/bin/env bash

cd Program
ls
rm dec-tree
make clean
make
cd ..


INSTANCES=('p01.txt' 'p02.txt' 'p03.txt' 'p04.txt' 'p05.txt' 'p06.txt' 'p07.txt' 'p08.txt' 'p09.txt' 'p10.txt')
for instance in "${INSTANCES[@]}" 
do 
    Program/dec-tree Datasets/$instance -depth 3 -t 600 -sol Solutions/$instance
done