from sys import argv, exit

if len(argv) < 2 or not argv[1].isdigit():
    exit(0)

sum_of_digits = sum([int(i) for i in argv[1]])
print(sum_of_digits)
