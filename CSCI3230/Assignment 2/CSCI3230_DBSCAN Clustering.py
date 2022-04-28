import matplotlib.pyplot as plt
import numpy as np
import math
from collections import defaultdict

class Graph:
    def __init__(self):
        self.graph = defaultdict(list)
    def addEdge(self, u, v):
        self.graph[u].append(v)
    def delEdge(self, u):
        self.graph[u].clear()
    def DFSUtil(self, v, visited):
        visited.add(v)
        for neighbour in self.graph[v]:
            if neighbour not in visited:
                self.DFSUtil(neighbour, visited)
    def DFS(self, v):
        visited = set()
        self.DFSUtil(v, visited)
        return visited

color=['g', 'r', 'b', 'g']
file = open("CSCI3230_ClusteringData.csv")
title = np.loadtxt(file, delimiter=",", dtype = str, max_rows=1)
pointData = np.loadtxt(file, delimiter=",")

file.close()
file = open("CSCI3230_initialCluster.csv")
initCluster = np.loadtxt(file, delimiter=",", skiprows=1)
file.close()

file = open("DBSCAN_Clustering Parameters.csv")
e, minPts = np.loadtxt(file, delimiter=",", skiprows=1)
dist = np.zeros((pointData.shape[0], pointData.shape[0]))
file.close()

core = np.zeros((pointData.shape[0]))
g = Graph()

for i, record1 in enumerate(pointData):
    dist[i][i] = -1
    for j, record2 in enumerate(pointData[:i]):
        if (((record1[0] - record2[0])**2 + (record1[1] - record2[1])**2)**0.5 <= e):
            dist[i][j] = -1
            dist[j][i] = -1
            g.addEdge(i, j)
            g.addEdge(j, i)
for i, record in enumerate(dist):
    print("%c's neighbour: " % chr(i+ord('a')), end = " ")
    for j, element in enumerate(record):
        if (element == -1):
            print("%c" % chr(j+ord('a')), end = " ")
    print()
print("Core points:", end = " ")
for i, record in enumerate(dist):
    if np.sum(record) <= -minPts:
         print("%c" % chr(i+ord('a')), end = " ")
         core[i] = -1
    else:
        g.delEdge(i)
print()
clusternum = -1
cluster = []
for i, isCore in enumerate(core):
    if (isCore == -1):
        cluster.append([])
        clusternum += 1
        cluster[clusternum] = g.DFS(i)
        for j, removecore in enumerate(cluster[clusternum]):
            core[removecore] = clusternum + 1
            pointData[removecore][2] = clusternum + 1
for i, corenum in enumerate(cluster):
    print(cluster[i])
for record in pointData:
    plt.scatter(record[0], record[1], c=color[int(record[2])])
plt.title("Q3d DBSCAN model")
plt.show()