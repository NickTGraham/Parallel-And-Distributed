#!/bin/bash
    for threads in 1 2 4 8 16 24 32 48 64
    do
        $HADOOP_HOME/bin/hadoop fs -cat /users/ngraham/"all$threads" >> tests/"all$threads"
    done
