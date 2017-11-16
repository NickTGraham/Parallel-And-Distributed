#!/bin/bash
for file in short.txt tom.txt paradise.txt pride.txt total.txt
do
    for threads in 1 2 4 8 16 24 32 48 64
    do
        $HADOOP_HOME/bin/hadoop jar wc.jar WordCount /users/ngraham/$file /users/ngraham/"$file$threads" -D mapred.reduce.tasks=$threads
    done
done
