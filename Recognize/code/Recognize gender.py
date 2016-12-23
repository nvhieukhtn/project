import datetime
import glob
import os
import warnings
import cv2
import numpy as np
from sklearn import svm
from sklearn.decomposition import PCA

warnings.filterwarnings("ignore")


class Svm:
    def __init__(self, datasets, sexs, kernel):
        self.datasets = datasets
        self.sexs = sexs
        self.svm_obj = svm.SVC(kernel=kernel, C=1.0)
        self.svm_obj.fit(datasets, sexs)

    def classify(self, examples):
        sexs_predict = self.svm_obj.predict(examples)
        return sexs_predict


class Data:
    def __init__(self, directory):
        self.directory = directory
        self.datasets = []
        self.sexs = []

    def load(self):
        os.chdir(self.directory)
        for image in glob.glob("*.jpg"):
            pixel = cv2.imread(image, cv2.CV_LOAD_IMAGE_GRAYSCALE)
            self.datasets.insert(self.datasets.__len__(), pixel.flatten())
        file_target = 'target.txt'
        self.sexs = np.loadtxt(fname=file_target, dtype=int)
        os.chdir('../..')


class Recognition:
    def __init__(self, component):
        self.pca = PCA(n_components=component, whiten=True)
        self.training = []
        self.sexs = []

    def train(self, datasets, sexs):
        self.pca.fit(datasets)
        self.training = self.pca.transform(datasets)
        self.sexs = sexs

    def predict(self, examples, sexs, kernel):
        results = self.pca.transform(examples)
        svm = Svm(self.training, self.sexs, kernel)
        sexs_predict = svm.classify(results)
        return self.caculate_accuracy(sexs, sexs_predict)

    def compare(self, sex, sex_predict):
        if sex == sex_predict:
            return 1
        return 0

    def caculate_accuracy(self, sexs, sexs_predict):
        correct = np.sum(np.array([self.compare(sexs[i], sexs_predict[i]) for i in range(len(sexs))]))
        accuracy = float(correct) / len(sexs)
        return accuracy


def test(folder):
    dir = 'test/' + folder
    test_datasets = Data(dir)
    test_datasets.load()
    print("============%s=================" % (folder.upper()))
    accuracy_1 = reg.predict(test_datasets.datasets, test_datasets.sexs, 'rbf')
    print("Kernel RBF \t Accuracy : %f " %(accuracy_1))
    accuracy_2 = reg.predict(test_datasets.datasets, test_datasets.sexs, 'poly')
    print("Kernel Poly \t Accuracy : %f " % (accuracy_2))
    accuracy_3 = reg.predict(test_datasets.datasets, test_datasets.sexs, 'sigmoid')
    print("Kernel Sigmoid \t Accuracy : %f " % (accuracy_3))
    accuracy_4 = reg.predict(test_datasets.datasets, test_datasets.sexs, 'linear')
    print("Kernel Linear \t Accuracy : %f " % (accuracy_4))


start = datetime.datetime.now().time()
os.chdir('..')
train_datasets = Data('train/gallery')
train_datasets.load()
reg = Recognition(30)
reg.train(train_datasets.datasets, train_datasets.sexs)

test('dup1')
test('dup2')
test('fb')
test('fc')
finish = datetime.datetime.now().time()
print("Time run : %d second" %((finish.minute - start.minute)*60 + finish.second - start.second))