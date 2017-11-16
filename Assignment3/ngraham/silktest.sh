for threads in 1 4 16 24 32 48 64 128
do
    for file in matrix_10.dat matrix_2000.dat
    do
      	for test in 1 2 3
        do
            ./silk $file $threads >> tests/cilk/"$file$us$threads$us$mem.out"
        done
    done
done
