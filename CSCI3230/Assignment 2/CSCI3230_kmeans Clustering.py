import matplotlib.pyplot as plt
import numpy as np
import math
color=['g', 'r', 'b']
tit = ["mm", "Assign Point Clusters", "Update Cluster Centroid"]
file = open("CSCI3230_ClusteringData.csv")
title = np.loadtxt(file, delimiter=",", dtype = str, max_rows=1)
pointData = np.loadtxt(file, delimiter=",")
addCol = np.ones((pointData.shape[0], 1))*1000
pointData = np.append(pointData, addCol, axis=1)
file.close()
file = open("CSCI3230_initialCluster.csv")
initCluster = np.loadtxt(file, delimiter=",", skiprows=1)
file.close()
iterationTime = 0
change = True
while change == True:
    change = False
    iterationTime += 1
    for row in initCluster:
        x1 = row[0]
        y1 = row[1]
        cNum = row[2]
        for record in pointData:
            dist = ((record[0] - x1)**2 + (record[1] - y1)**2)**0.5
            if dist < record[3]:
                record[3] = dist
                record[2] = cNum
                change = True
    print("Iteration %d:" % iterationTime)
    print(pointData)
    for record in pointData:
        plt.scatter(record[0], record[1], c=color[int(record[2])])  
    for row in initCluster:
        plt.scatter(row[0], row[1], marker='x', c=color[int(row[2])]) 
    #plt.title("Iteration %d" % iterationTime)
    plt.title(tit[iterationTime])
    plt.show()
    clusterCount = np.zeros(initCluster.shape[0])
    for row in initCluster:
        row[0] = 0
        row[1] = 0
    for record in pointData:
        initCluster[int(record[2])-1][0] += record[0]
        initCluster[int(record[2])-1][1] += record[1]
        clusterCount[int(record[2])-1] += 1
    for row, count in zip(initCluster, clusterCount):
        row[0] /= count
        row[1] /= count
        print("%f %f" % (row[0], row[1]))
    for record in pointData:
        record[3] = ((record[0] - initCluster[int(record[2])-1][0])**2 + (record[1] - initCluster[int(record[2])-1][1])**2)**0.5 
print("Iteration ends.")        

   
