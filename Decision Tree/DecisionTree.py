
# coding: utf-8

#     Tiêu đề   : Bài tập cá nhân 02
#     Họ và tên : Nguyễn Văn Hiếu
#     MSSV      : 1412166
#     Phiên bản : Python 2.7
# 
# ## Xây dựng  thuật toán cây quyết định cho bài toán phân lớp với tập dữ liệu iris
# 1. Xấy dựng các lớp đối tượng
#     - Lớp $\color{red}{Node}$:
#         - Lưu giá trị tại một nút trên cây quyết định
#         - Tại mỗi Node có một thuộc tính feature
#         - Mỗi Node có hoặc nhiều đối tượng Branch
#     - Lớp $\color{red}{Branch}$: 
#         - Lưu các nhánh đi tại một nút
#         - Mỗi Branch gồm có một giá trị value là giá trị tại nút đó, và nút con kiểu Node
#     - Lớp $\color{red}{DecisionTree}$ bao gồm:
#         - Các hàm tạo cây quyết định
#         - Các hàm kiểm tra độ chính xác của cây quyết định đã tạo
# 2. Xây dựng cây quyết định
#     - $Entropy = \sum\limits_{i = 1}^{c}(-p_ilog_2p_i)$ là hàm để đo độ thuần của tập train
#     - $Gain(S,A) = \sum\limits_{v \in values(A)}{{\mid{S_v}\mid}\over{\mid{S_v}\mid}}$ là hàm kết hợp hàm Entropy để đơn giản hóa cây quyết định
#     - Hàm Best-Split: Tìm ra được trường thuộc tính có độ lợi nhỏ nhất để chia
#            Input: Tập dữ liệu và tập thuộc tính
#            Ouput: Thuộc tính chia có lợi nhất
#            **************************************
# ```python
# def Best_Split(Data,F):
#     min_gain = 1;
#     for f in F:
#         if(Gain(data,f) < min_gain):
#             min_gain = Gain(data,f)
#             best_split = f
#     return best_split
# ```
#     - Thuật toán ID3
# ```python
# def GrowTree(D,F):
#    if(Homogeneous(D)): #Kiểm tra lớp đã được phân hay chưa
#        return Label(D) #Trả về nhãn của Target
#    S = Best-Split(D)
#    for data in dataCollection: # dataCollection là data bị chia bởi S
#        if(data != NULL):
#            T = GrowTree(data,F)
#        else:
#            return Label(D)
#    return S #Với các cây con T
# ```
# ### Một số hàm quan trọng
# 
# ```python
# def ConvertDataset(D): #Chuyển dữ liệu từ số thực sang giá trị rời rạc
# def GetTarget(D): #Từ cây quyết định đã tạo và tập dữ liệu cho vào xác định lớp của nó
# ```
# 3. Chuẩn hóa dữ liệu Iris
#     - Tập dữ liệu Iris có dạng :
#     
# |Sepal length|Sepal width|Petal length|Petal width|Class|
# |:----------:|:---------:|:----------:|:---------:|:---:|
# |     R      |     R     |     R      |     R     |Text |
# 
#     - Với các trường là số thực, vì vậy ta cần chuyển các trường đó thành dữ liệu rời rạc
#     
# |Sepal length|Sepal width|Petal length|Petal width|
# |:----------:|:---------:|:----------:|:---------:|
# |>5.95       |>3.1       |>2.5        |>1.75      |
# |4.95<x<=5.95|3.05<x<=3.1|<= 2.5      |<=1.75     |
# |<4.95       |<3.05      | 
# 
# 
# 4. Tập dữ liệu
#     - Dữ liệu train
#         - Sử dụng 50% dữ liệu iris để train. Lấy các row chẵn để train (0,2,4,...,148)
#     - Dữ liệu test
#         - Sử dụng 50% dữ liệu iris còn lại để test. Lấy các row lẻ để test (1,3,5,..,149)
# 5. Kết quả 
#     - Training
#         - Đúng 73/75 bộ mẫu. 2 mẫu sai.
#         - Độ chính xác 97%
#     - Testing
#         - Đúng 72/75 bộ mẫu
#         - Độ chính xác 96%

# In[6]:

import math
class Branch:
    def __init__(self, val, chil):
        self.value = val
        self.child = chil

class Node:
    def __init__(self, c):
        self.category = c
        self.branch = []
    def AddBranch(self,way):
        self.branch.append(way)
    def IsLeaf(self):
        if len(self.branch) == 0:
            return True
        else:
            return False
class DecisionTree:
    def __init__(self):
        self.ErrorCount = 0
    def Entropy(self, data,feature,value):
        valueEntropy = 0.0
        subData = data[data[feature] == value]
        num_records = len(subData)
        num_categories = subData[target].value_counts()
        coeffident = [x for x in num_categories]
        for x in coeffident:
            temp = float(x)/num_records
            valueEntropy += (-1) * temp * math.log(temp, 2)
        return valueEntropy
    def Gain(self, data,feature):
        dataGain = 0.0
        num_records = len(data)
        subData = data[[feature, target]].groupby(feature).count()
        for category in subData.index:
            count_category = float(len(data[data[feature] == category]))
            dataGain += (count_category / num_records) * self.Entropy(data, feature, category)
        return dataGain
    def Homogeneous(self, data):
        if(len(data.columns) == 1):
            return True
        categories = data[target].value_counts()
        if (len(categories) == 1):
            return True
        else:
            return False

    def Label(self, data):
        max_value = 0
        subData = data[[target]].groupby(target).count().index
        for result in subData:
            if len(data[data[target] == result]) > max_value:
                max_value = len(data[data[target] == result])
                leaf = result
        self.ErrorCount += len(data) - max_value
        leaf = Node(leaf)
        return leaf
    def BestSplit(self, data, features):
        min_gain = 1000
        for feature in features:
            gain = self.Gain(data, feature)
            if (gain < min_gain):
                min_gain = gain
                best_feature = feature
        return best_feature

    def GrowTree(self, data, features):
        if self.Homogeneous(data):
            return self.Label(data)
        best_feature = self.BestSplit(data, features)
        root = Node(best_feature)
        value_of_features = data[best_feature].value_counts().index.values
        for value in value_of_features:
            subData = data[data[best_feature] == value].drop(best_feature, axis=1)
            num_subcolumns = len(subData.columns)
            subFeatures = subData.columns[:num_subcolumns - 1]
            node = self.GrowTree(subData, subFeatures)
            way = Branch(value, node)
            root.AddBranch(way)
        return root
    def MakeTree(self,data,features):
        self.Tree = self.GrowTree(data,features)
        return self.ErrorCount
    def GetTarget(self,data_row):
        tmpTree = self.Tree
        while tmpTree.IsLeaf() == False:
            for val in tmpTree.branch:
                if len(data_row[data_row[tmpTree.category] == val.value]) > 0:
                    tmpTree = val.child
                    break
        return tmpTree.category
    def Test(self,data_test):
        self.ErrorCount = 0
        num_records = len(data_test)
        for i in range(num_records):
            data_row = data_test.iloc[[i]]
            result = self.GetTarget(data_row)
            if len(data_row[data_row[target] == result]) == 0:
                self.ErrorCount += 1
        return self.ErrorCount
def convertData(data,features):
    data.loc[data[features[0]] > 5.95,features[0]] = 'Sepal length > 5.95'
    data.loc[data[features[0]] <= 5.95,features[0]] = '4.95 < Sepal length <= 5.95'
    data.loc[data[features[0]] <= 4.95,features[0]] = 'Sepal length <= 4.95'

    data.loc[data[features[1]] > 3.1,features[1]] = 'Sepal width > 3.1'
    data.loc[data[features[1]] <= 3.1, features[1]] = '3.05 < Sepal width <= 3.1'
    data.loc[data[features[1]] <= 3.05, features[1]] = 'Sepal width <= 3.05'

    data.loc[data[features[2]] > 2.5, features[2]] = 'Petal length > 2.5'
    data.loc[data[features[2]] <= 2.5, features[2]] = 'Petal length <= 2.5'

    data.loc[data[features[3]] > 1.75,   features[3]] = 'Petal Width > 1.75'
    data.loc[data[features[3]] <= 1.75, features[3]] = 'Petal Width <= 1.75'
    return data
#Main function
import pandas as pd
import numpy as np
data = pd.read_csv('http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data',header=None)
data.columns = ["Sepal length","Sepal width","Petal length","Petal width","Class"]
target = "Class"
num_columns = len(data.columns) - 1
features = data.columns[:num_columns]
data = convertData(data,features)
num_columns = len(data.columns) - 1
features = data.columns[:num_columns]
data_train = data[0:150:2]
data_test = data[1:150:2]

decisionTree = DecisionTree()
ErrorCount_Training = decisionTree.MakeTree(data_train,features)
print("So tap sai khi Training : ",ErrorCount_Training, "Tong so tap : " ,len(data_train))
print("Do chinh xac : ",float(100*(len(data_train) - ErrorCount_Training )/ len(data_train)),"%")

ErrorCount_Testing = decisionTree.Test(data_test)
print("So tap sai khi Testing : ",ErrorCount_Testing,'.Tong so tap : ',len(data_test))
print("Do chinh xac : ",float(100*(len(data_test) - ErrorCount_Testing )/ len(data_test)),"%")

