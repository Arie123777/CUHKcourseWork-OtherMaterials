import matplotlib.pyplot as plt
import numpy as np
max = -99999.
min = 99999.
v2 = int(input("How many variable? "))
xTrain = np.ones((v2, 1))
yTrain = np.ones((v2, 1))
for v in range (v2):
    xTrain[v][0], yTrain[v][0] = input("").split()
    if (xTrain[v][0] > max):
        max = xTrain[v][0]
    if (xTrain[v][0] < min):
        min = xTrain[v][0]
v3 = int(input("How many tests? "))
if (v3 > 0):
    xTest = np.ones((v3, 1))
    yTest = np.ones((v3, 1))
    expectedY = np.zeros((v3, 1))
    for v in range (v3):
        xTest[v][0], yTest[v][0] = input("").split()
        if (xTest[v][0] > max):
            max = xTest[v][0]
    if (xTrain[v][0] < min):
            min = xTest[v][0]
testis = int(input("How many testis? "))
if (v3 > 0):
    predictedY = np.zeros((v3, 1))
for test in range (testis):
    v1 = int(input("How many theta? "))
    A = np.ones((v2, v1))
    for i in range (v2):
        plt.scatter(xTrain[i][0], yTrain[i][0], label= "stars", color= "green", marker= "*", s=30)
        A[i][1] = xTrain[i][0]
    for i in range(v2):
        #A[i][1], B[i][0] = input("%d : input a, b: " % (i + 1)).split()
        for j in range(2, v1):
            A[i][j] = A[i][j-1] * A[i][1]
    AT = A.transpose()
    Theta = np.matmul(np.matmul(np.linalg.inv(np.matmul(AT, A)), AT), yTrain)
    print("Theta is: ")
    print(Theta)
    B = np.ones((v3, v1))
    for i in range(v3):
        B[i][1] = xTest[i][0]
        for j in range(2, v1):
            B[i][j] = B[i][j-1] * B[i][1]
    if (v3 > 0):
        predictedY = np.matmul(B, Theta)
        expectedY = np.ones((v3, 1)) * (np.sum(predictedY)/v3)
    print (predictedY)
    print("EPE: %f" % np.sum((yTest - predictedY)**2 ))
    x = np.linspace(min, max, 100000)
    y = 0.0
    for i in range(v1):
        y = y + Theta[i][0] * x ** i
    plt.plot(x, y)
    plt.xlabel('x - axis')
    plt.ylabel('y - axis')
    plt.title('1%c solution' % chr(ord('a') + test))
    plt.show()
    
    #print(np.square(np.subtract(np.matmul(A, Theta), B)).shape)
    #print(np.subtract(np.matmul(A, Theta), B))
    #varience = np.sum(np.square(np.subtract(np.matmul(A, Theta), B)))/v2
    #print(varience)
#wantTest = input("Want test? (Y/Other Key) ")
#if wantTest == 'Y':
#    testNum = int(input("How many test you want to do? "))
#    actual = input("Do you have actual result?")
#    if actual == 'Y':
#        for i in range (testNum):
            