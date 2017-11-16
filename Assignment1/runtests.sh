for threads in 1 4 16 24 32 54
do
    for inc in 1 100 10000 1000000 5000000
    do
        for i in 1 2 3 4 5
        do
            ./parcount -t $threads -i $inc
            echo " "
        done
    done
    echo "\n"
done
