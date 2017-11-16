import random
f = open("matrix_5000.dat", 'w')
i = 5000
f.write(str(i) + " " + str(i) + " " + str(i*i) + "\n");

for j in range(1, i+1):
    print(j)
    for k in range(1, i+1):
        f.write(str(j) + " " + str(k) + " " + str(random.randint(0, j)) + "\n");

f.write(str(0) + " " + str(0) + " " + str(0) + "\n");
