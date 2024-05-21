import time
timestamp = time.strftime('%H:%M:%S')
print(timestamp)
hour = int(time.strftime("%H"))
if (hour < 5):
  print("It's still night sir")
elif (hour < 12):
  print("Good Morning Sir")
elif (hour < 18):
  print("Good Afternoon Sir")
elif (hour < 20):
  print("Good Evening Sir")
else:
  print("Good night sir")