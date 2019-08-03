start_x = int(input())
start_y = int(input())
finish_x = int(input())
finish_y = int(input())
if (start_x + start_y) % 2 == (finish_x + finish_y) % 2:
    print('YES')
else:
    print('NO')
