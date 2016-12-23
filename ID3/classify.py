import math
import pandas as pd
import sys


class Branch:
    def __init__(self, value, child):
        self.value = value
        self.child = child


class Node:
    def __init__(self, category):
        self.category = category
        self.branches = []

    def add_branch(self, branch):
        self.branches.append(branch)

    def is_leaf(self):
        return len(self.branches) == 1 and self.branches[0].child is None

    def export_tree(self, level, f_file):
        tabs = '    '
        for branch in self.branches:
            text = tabs * level + self.category + " : " + str(branch.value)
            f_file.write(text + '\n')
            if branch.child is not None:
                branch.child.export_tree(level + 1, f_file)


class DecisionTree:
    def __init__(self):
        pass

    @staticmethod
    def entropy(dataset, feature, value):
        target = dataset.columns[len(dataset.columns) - 1]
        value_entropy = 0.0
        sub_data = dataset[dataset[feature] == value]
        num_records = len(sub_data)
        num_fields = sub_data[target].value_counts()
        coefficent = [x for x in num_fields]
        for x in coefficent:
            temp = float(x) / num_records
            value_entropy += (-1) * temp * math.log(temp, 2)
        return value_entropy

    def gain(self, dataset, category):
        data_gain = 0.0
        num_records = len(dataset)
        target = dataset.columns[len(dataset.columns) - 1]
        sub_data = dataset[[category, target]].groupby(category).count()
        for value in sub_data.index:
            count_category = float(len(dataset[dataset[category] == value]))
            data_gain += (count_category / num_records) * self.entropy(dataset, category, value)
        return data_gain

    @staticmethod
    def homogeneous(dataset):
        target = dataset.columns[len(dataset.columns) - 1]
        if len(dataset.columns) == 1:
            return True
        fields = dataset[target].value_counts()
        return len(fields) == 1

    @staticmethod
    def label(dataset):
        max_value = 0
        target = dataset.columns[len(dataset.columns) - 1]
        sub_data = dataset[[target]].groupby(target).count().index
        for result in sub_data:
            if len(dataset[dataset[target] == result]) > max_value:
                max_value = len(dataset[dataset[target] == result])
                value = result
        node = Node(target)
        branch = Branch(value, None)
        node.add_branch(branch)
        return node

    def best_split(self, dataset):
        min_gain = 1000
        for category in dataset.columns[:len(dataset.columns) - 1]:
            gain = self.gain(dataset, category)
            if gain < min_gain:
                min_gain = gain
                best_category = category
        return best_category

    def grow_tree(self, dataset):
        if self.homogeneous(dataset):
            return self.label(dataset)
        best_feature = self.best_split(dataset)
        root = Node(best_feature)
        value_of_features = dataset[best_feature].value_counts().index.values
        for value in value_of_features:
            sub_data = dataset[dataset[best_feature] == value].drop(best_feature, axis=1)
            node = self.grow_tree(sub_data)
            branch = Branch(value, node)
            root.add_branch(branch)
        return root

    def make_tree(self, dataset):
        self.root = self.grow_tree(dataset)

    def get_target(self, data_row):
        tmp_tree = self.root
        while not tmp_tree.is_leaf():
            is_next = False
            for branch in tmp_tree.branches:
                if data_row[tmp_tree.category] == branch.value:
                    tmp_tree = branch.child
                    is_next = True
                    break
            if not is_next:
                return None
        return tmp_tree.branches[0].value

    def predict(self, dataset, out_pr):
        f = open(out_pr, 'w')
        for i in range(len(dataset)):
            data_row = dataset.loc[i]
            result = self.get_target(data_row)
            if result is None:
                result = "Unknown"
            f.write(result + '\n')
        f.close()

    def export_tree(self, out_tree):
        f = open(out_tree, 'w')
        self.root.export_tree(0, f)
        f.close()


class Parameter:
    def __init__(self):
        for i in range(1, len(sys.argv)):
            if "train=" in sys.argv[i]:
                self.train = str(sys.argv[i]).split("=")[1]
            if "test=" in sys.argv[i]:
                self.test = str(sys.argv[i]).split("=")[1]
            if "out_tree=" in sys.argv[i]:
                self.out_tree = str(sys.argv[i]).split("=")[1]
            if "out_pr=" in sys.argv[i]:
                self.out_pr = str(sys.argv[i]).split("=")[1]


parameter = Parameter()

data_train = pd.read_csv(parameter.train)
decision_tree = DecisionTree()
decision_tree.make_tree(data_train)
decision_tree.export_tree(parameter.out_tree)
data_test = pd.read_csv(parameter.test)
decision_tree.predict(data_test, parameter.out_pr)
