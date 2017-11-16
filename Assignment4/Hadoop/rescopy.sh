#!/bin/bash
for file in short.txt tom.txt paradise.txt pride.txt total.txt
do
    for threads in 1 2 4 8 16 24 32 48 64
    do
        $HADOOP_HOME/bin/hadoop fs -cat /users/ngraham/"$file$threads" >> tests/"$file$threads"
    done
done
