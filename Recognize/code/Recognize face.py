import datetime
import glob
import math
import os
import warnings

import cv2
import numpy as np
from sklearn.decomposition import PCA

warnings.filterwarnings("ignore")


class Distance:
    @staticmethod
    def city_block(example_1, example_2):
        return np.sum(np.abs(example_1 - example_2))

    @staticmethod
    def euclidean(example_1, example_2):
        return np.sum(np.abs(example_1 - example_2)**2)

    @staticmethod
    def cosine(example_1, example_2):
        sum_mul = np.sum(np.array(example_1)*np.array(example_2))
        sum_square_x = np.sum(np.array(example_1)**2)
        sum_square_y = np.sum(np.array(example_2)**2)
        return (-1.0) * sum_mul / math.sqrt(sum_square_x * sum_square_y)


    @staticmethod
    def mahalanobis(a, b):
        return 0


class Knn:
    def __init__(self, distance, datasets, names):
        self.func = distance
        self.datasets = datasets
        self.names = names

    def predict(self, example):
        weights = np.array([self.func(example, dataset) for dataset in self.datasets])
        index = np.argmin(weights)
        return self.names[index]

    def classify(self, examples):
        names_predict = np.array([self.predict(example) for example in examples])
        return names_predict


class Data:
    def __init__(self, directory):
        self.directory = directory
        self.images = []
        self.datasets = []

    def load(self):
        os.chdir(self.directory)
        for image in glob.glob("*.jpg"):
            self.images.insert(self.images.__len__(), image)
            pixel = cv2.imread(image, cv2.CV_LOAD_IMAGE_GRAYSCALE)
            self.datasets.insert(self.datasets.__len__(), pixel.flatten())
        os.chdir('../..')


class Recognition:
    def __init__(self, component):
        self.pca = PCA(n_components=component, whiten=True)
        self.training = []
        self.names = []

    def train(self, datasets, names):
        self.pca.fit(datasets)
        self.training = self.pca.transform(datasets)
        self.names = names

    def predict(self, examples, distance,names):
        results = self.pca.transform(examples)
        knn = Knn(distance, self.training, self.names)
        names_predict = knn.classify(results)
        return self.caculate_accuracy(names, names_predict)

    def compare(self, name, name_predict):
        if name[0:5] == name_predict[0:5]:
            return 1
        return 0

    def caculate_accuracy(self, names, names_predict):
        correct = np.sum(np.array([self.compare(names[i], names_predict[i]) for i in range(len(names))]))
        accuracy = float(correct) / len(names)
        return accuracy


def test(folder):
    dir = 'test/' + folder
    test_datasets = Data(dir)
    test_datasets.load()
    print("============%s=================" %(folder.upper()))
    accuracy_1 = reg.predict(test_datasets.datasets, Distance.city_block, test_datasets.images)
    print("Distance Function : %s\tAccuracy : %f\n" %("City Block", accuracy_1))

    accuracy_2 = reg.predict(test_datasets.datasets, Distance.euclidean, test_datasets.images)
    print("Distance Function : %s\tAccuracy : %f\n" % ("Euclidean", accuracy_2))

    accuracy_3 = reg.predict(test_datasets.datasets, Distance.cosine, test_datasets.images)
    print("Distance Function : %s\tAccuracy : %f\n" % ("Cosine", accuracy_3))

start = datetime.datetime.now().time()
os.chdir('..')
train_datasets = Data('train/gallery')
train_datasets.load()
reg = Recognition(50)
reg.train(train_datasets.datasets, train_datasets.images)

test('dup1')
test('dup2')
test('fb')
test('fc')
finish = datetime.datetime.now().time()
print("Time run : %d second" %((finish.minute - start.minute)*60 + finish.second - start.second))