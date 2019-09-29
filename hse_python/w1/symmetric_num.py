num = int(input())
left = num // 1000
right = num % 10
m_left = num // 100 % 10
m_right = num // 10 % 10
res = (left - right)**2
res *= 100
res = res + (m_left - m_right)**2
res *= 100
print(1 - res)
