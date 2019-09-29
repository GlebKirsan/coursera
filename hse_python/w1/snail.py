h = int(input())
up = int(input())
down = int(input())
counter = 1
h -= up
speed = up - down
print(counter + (h + speed - 1) // speed)
