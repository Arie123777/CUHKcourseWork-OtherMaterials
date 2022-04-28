import matplotlib.pyplot as plt
import numpy as np
import math

f = open('LogisticRegression.txt', 'r')
readTrain = True
line = f.readline()
inputList = line.split()
mapArray = list(map(float, inputList))
train = np.array([mapArray])
while line.strip():
    line = f.readline()
    if (line.strip()):
        inputList = line.split()
        mapArray = np.array(list(map(float, inputList)))
        train = np.concatenate((train,[mapArray]),axis= 0)
print(train)
print(np.shape(train))

line = f.readline()
inputList = line.split()
mapArray = list(map(float, inputList))
test = np.array([mapArray])
while line.strip():
    line = f.readline()
    if (line.strip()):
        inputList = line.split()
        mapArray = np.array(list(map(float, inputList)))
        test = np.concatenate((test,[mapArray]),axis= 0)
print(test)
print(np.shape(test))

line = f.readline()
mapArray = np.array([float(line)])
theta = np.array([mapArray])
while line.strip():
    line = f.readline()
    if (line.strip()):
        mapArray = np.array([float(line)])
        theta = np.concatenate((theta,[mapArray]),axis= 0)
print(theta)
learnRate = float(f.readline())
print(learnRate)
xTrain = train[:, :np.shape(train)[1]-1]
yTrain = np.transpose([train[:, np.shape(train)[1]-1]])
print(xTrain)
print(yTrain)
xTest = test[:, :np.shape(test)[1]-1]
yTest = np.transpose([test[:, np.shape(test)[1]-1]])
print(xTest)
print(yTest)
XT = np.insert(xTrain, 0, np.ones(np.shape(xTrain)[0]), 1)
X = np.transpose(XT)
summation = np.zeros((np.shape(X)[0], 1))
print(summation)
for i in range(0,np.shape(X)[1]):
    XiT = np.array([X[:, i]])
    Xi = np.array(np.transpose(XiT))
    yi = yTest[i][0]
    print(XiT)
    #print((1/(1+math.e ** -np.matmul(XiT, theta) - yi)) * Xi)
    summation = summation + (1/(1+math.e ** np.matmul(-XiT, theta)) - yi) * Xi
    print((1/(1+math.e ** np.matmul(-XiT, theta) - yi)) * Xi)
print(theta-learnRate*summation)
print(X)

f.close()
