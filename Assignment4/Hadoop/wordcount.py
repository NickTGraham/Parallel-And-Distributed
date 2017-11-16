import sys
count = 0
fname = sys.argv[1]
with open(fname) as f:
    content = f.readlines()
    for line in content:
        count = count+len(line.split(" "))
print(count)
