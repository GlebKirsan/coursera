rubles = int(input())
cents = int(input())
N = int(input())
cents *= N
rubles = rubles * N + cents // 100
print(rubles, cents % 100)
