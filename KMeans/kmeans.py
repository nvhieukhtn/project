# coding=utf-8
import sys
import pandas as pd
from math import pow
from math import sqrt
import numpy as np


class Parameter:
    def __init__(self):
        self.input = ''
        self.out = ''
        self.k = 0
        for i in range(1, len(sys.argv)):
            if "inp=" in sys.argv[i]:
                self.input = str(sys.argv[i]).split("=")[1]
            if "k=" in sys.argv[i]:
                self.k = int(str(sys.argv[i]).split("=")[1])
            if "out=" in sys.argv[i]:
                self.out = str(sys.argv[i]).split("=")[1]

    def check_validation(self):
        if self.input == '' or self.out == '' or self.k <= 0:
            return False
        return True


class Transaction:
    def __init__(self, transaction_id, data_item):
        self.transaction_id = transaction_id
        self.cluster_id = 0
        self.data = data_item

    def get_distance(self, origin):
        distance = 0
        for i in range(len(origin)):
            distance += pow(origin[i] - self.data[i], 2)
        distance = sqrt(distance)
        return distance


class Cluster:
    def __init__(self, cluster_id, mean):
        self.cluster_id = cluster_id
        self.mean = mean
        self.elements = []

    def update_mean(self):
        total = np.array(list(self.elements[0].data), dtype='float')
        pos = 1
        while pos < self.elements.__len__():
            total += self.elements[pos].data
            pos += 1
        total /= self.elements.__len__()
        self.mean = total

    def add(self, element):
        self.elements.append(element)

    def remove(self, index):
        del self.elements[index]


class KMean:
    def __init__(self, k, dataset):
        if k > len(dataset):
            print("k must less than length of dataset")
            sys.exit(0)
        self.num_of_clusters = k
        self.clusters = []
        self.dataset = []
        self.max_loop = 100   # Chặn trường hợp lặp vô hạn
        for i in range(len(dataset)):
            transaction = Transaction(i, dataset.loc[i])
            self.dataset.insert(len(self.dataset), transaction)
        for i in range(k):
            self.clusters.insert(len(self.clusters), Cluster(i + 1, dataset.loc[i]))
        for i in range(len(self.dataset)):
            iCluster = self.get_cluster(self.dataset[i])
            self.clusters[iCluster].add(self.dataset[i])
            self.dataset[i].cluster_id = self.clusters[iCluster].cluster_id

    def learn(self):
        loop = True
        count = 0
        while loop and count <= self.max_loop:
            loop = False
            count += 1
            for iCluster in range(self.num_of_clusters):
                self.clusters[iCluster].update_mean()
            for iCluster in range(self.num_of_clusters):
                iNode = 0
                while iNode < len(self.clusters[iCluster].elements):
                    iNewCluster = self.get_cluster(self.clusters[iCluster].elements[iNode])
                    if iNewCluster != iCluster and len(self.clusters[iNewCluster].elements) > 1:
                        loop = True
                        self.clusters[iNewCluster].add(self.clusters[iCluster].elements[iNode])
                        self.dataset[self.clusters[iCluster].elements[iNode].transaction_id].cluster_id = self.clusters[
                            iNewCluster].cluster_id
                        self.clusters[iCluster].remove(iNode)
                    iNode += 1

    def get_cluster(self, sample):
        best_cluster = 0
        min_distance = sample.get_distance(self.clusters[0].mean)
        for iCluster in range(1, self.num_of_clusters):
            distance = sample.get_distance(self.clusters[iCluster].mean)
            if distance < min_distance:
                min_distance = distance
                best_cluster = iCluster
        return best_cluster

    def export_clusters(self, filename):
        file_writer = open(filename, 'w')
        for i in range(len(self.dataset)):
            file_writer.write(str(self.dataset[i].cluster_id) + '\n')
        file_writer.close()


params = Parameter()
if not params.check_validation():
    print("Syntax error!")
    sys.exit(0)
data = pd.read_csv(params.input)
kmean = KMean(params.k, data)
kmean.learn()
kmean.export_clusters(params.out)
