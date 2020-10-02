from sys import argv, exit

if len(argv) < 4 or not \
        (argv[1].isdigit() or argv[2].isdigit() or argv[3].isdigit()):
    exit(0)

a = int(argv[1])
b = int(argv[2])
c = int(argv[3])

d_sqrt = (b**2 - 4*a*c)**0.5
denominator = 2 * a
x1 = (-b + d_sqrt) / denominator
x2 = (-b - d_sqrt) / denominator
print(int(x1), int(x2))
