#!/bin/bash
for file in short.txt tom.txt paradise.txt pride.txt total.txt
do
    /u/hdusr/hadoop/bin/hdfs dfs -put $file /users/ngraham/$file
done
