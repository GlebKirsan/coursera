a = int(input())
b = int(input())
whole = a % b
norm_whole = 2 * whole // (whole + 1)
print('YES' * (1 - norm_whole), 'NO' * norm_whole, sep='')
