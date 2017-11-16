#!/bin/bash
for file in matrix_10.dat matrix_2000.dat
do
    for threads in 1 2 4 8 16 24 32 48 64
    do
        for test in 1 2 3
        do
	    echo $threads
	    echo $file
            mpirun -np $threads -machinefile hosts linux/sor $file >> tests/"$file$threads".data
        done
    done
done
