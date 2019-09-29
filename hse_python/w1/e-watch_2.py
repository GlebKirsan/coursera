seconds = int(input())
minutes = seconds // 60
seconds -= minutes * 60
hours = minutes // 60
minutes -= hours * 60
print(f'{hours % 24}:{minutes:02}:{seconds:02}')
