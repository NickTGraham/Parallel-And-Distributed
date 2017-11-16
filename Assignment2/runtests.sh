for threads in 1 4 16 24 32 48 64
do
    for inc in 100 1000 10000 #100000
    do
        ./parcount -t $threads -i $inc
    done
done
