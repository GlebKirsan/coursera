num = int(input())
left = num // 1000
right = num % 10
m_left = num // 100 % 10
m_right = num // 10 % 10
res = left - right
res = res + m_left - m_right
print(res + 1)
