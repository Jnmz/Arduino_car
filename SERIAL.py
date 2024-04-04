import datetime
import serial
import matplotlib.pyplot as plt


Ser=serial.Serial('COM3', 9600)
if Ser.isOpen():
    print("Serial port opened")
else:
    print("Serial port opening failed")
DataList=[]
nowtime=datetime.datetime.now()
while datetime.datetime.now() - nowtime < datetime.timedelta(seconds=30):
    Data=Ser.readline().decode('utf-8').replace('\r\n','')
    Elements=Data.split(',')
    if len(Elements) != 3:
        Elements = [0] * 3
    DataList.append((float(Elements[0]), float(Elements[1]), float(Elements[2])))
Ser.close()
print(DataList)

# Initial position and direction
x = 0
y = 0
direction = "north"  # Assuming initially heading due north
trajectory = [(x, y)]

# Update position and direction based on ultrasonic data
for cm1, cm2, cm3 in DataList:
    if (cm1 <= 30 and cm2 <= 30 and cm3 > 30) or (cm1 <= 30 and cm2 > 30 and cm3 > 30):  # turn right
        if direction == "north":
            direction = "east"
        elif direction == "east":
            direction = "south"
        elif direction == "south":
            direction = "west"
        elif direction == "west":
            direction = "north"
    elif (cm1 > 30 and cm2 <= 30 and cm3 <= 30) or (cm1 > 30 and cm2 > 30 and cm3 <= 30) or (cm1 > 30 and cm2 <= 30 and cm3 > 30) or (cm1>30 and cm2>30 and cm3>30):  # 直行
        if direction == "north":
            y += 1
        elif direction == "east":
            x += 1
        elif direction == "south":
            y -= 1
        elif direction == "west":
            x -= 1
    elif (cm1 <= 30 and cm2 > 30 and cm3 <= 30) or (cm1 <= 30 and cm2 <= 30 and cm3 <= 30):  # turn left
        if direction == "north":
            direction = "west"
        elif direction == "east":
            direction = "north"
        elif direction == "south":
            direction = "east"
        elif direction == "west":
            direction = "south"

    trajectory.append((x, y))
print(trajectory)

# Write trajectory data to a text file
with open(r"C:\Users\admin\Desktop\neccessary_file\path\way.txt", "w") as file:
    for x, y in trajectory:
        file.write(f"{x},{y}\n")

print("The trajectory data has been written to the way.txt file")

# Draw trajectory
x_values, y_values = zip(*trajectory)
plt.plot(x_values, y_values, marker='o')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('way')
plt.grid(True)
plt.show()
