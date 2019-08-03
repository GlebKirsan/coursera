first_floor = int(input())
last_floor = int(input())
prev_floor = first_floor - 1
flat_num = last_floor - prev_floor
if prev_floor % flat_num == 0:
    print('YES')
else:
    print('NO')
