n = int(input())
m = int(input())
whole_days = m // n
m -= whole_days * n
rest = n - m
print(whole_days + ((rest // n) + 1) % 2)
