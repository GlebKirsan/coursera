from sys import argv, exit

if len(argv) < 2 or not argv[1].isdigit():
    exit(0)

stairs_number = int(argv[1])
[print(' ' * (stairs_number - i) + '#' * i) for i in range(1, stairs_number + 1)]
