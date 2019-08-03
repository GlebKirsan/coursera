start_x = int(input())
start_y = int(input())
diff_x = int(input()) - start_x
diff_y = int(input()) - start_y
if diff_x < 0:
    diff_x = -diff_x
if diff_y < 0:
    diff_y = -diff_y

if diff_x <= 1 and diff_y <= 1:
    print('YES')
else:
    print('NO')
