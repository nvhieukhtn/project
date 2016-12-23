from math import ceil
class Transaction:
    def __init__(self, min_sup, input):
        file = open(input, 'r')
        self.data = []
        for line in file:
            line = line.replace('\n', '')
            items = line.split(',')
            self.data.insert(self.data.__len__(), items)
        file.close()
        self.min_record = ceil(min_sup * self.data.__len__())
    def get_itemset(self):
        itemsets = []
        for line in self.data:
            for item in line:
                if (itemsets.count([item]) == 0):
                    itemsets.insert(itemsets.__len__(), [item])
        itemsets.sort()
        return itemsets
    def get_support(self,cadidate):
        num_support = 0
        for sample in self.data:
            isContain = True
            for item in cadidate:
                if sample.count(item) == 0:
                    isContain = False
                    break
            if isContain == True:
                num_support += 1
        return num_support
    def to_string(self, item):
        text = ''
        for i in range(item.__len__() - 1):
            text += item[i] + ','
        text += item[item.__len__() - 1] + '\t' + str(
            float(self.get_support(item)) / self.data.__len__() * 100) + '%\n'
        return text

class Apriori:
    def __init__(self, min_sup, input):
        self.min_sup = min_sup
        self.transaction = Transaction(min_sup, input)
        self.freqItemset = []
        self.candidate = []
    def is_valid(self, f1, f2):
        for i in range(f1.__len__() - 1):
            if f1[i] != f2[i]:
                return False
        if f1[f1.__len__() - 1] >= f2[f2.__len__() - 1]:
            return False
        return True
    def cadidate_gen(self):
        self.candidate = self.transaction.get_itemset()
        curr_candidate = []
        for i in range(self.candidate.__len__()):
            if self.transaction.get_support(self.candidate[i]) >= self.transaction.min_record:
                curr_candidate.insert(curr_candidate.__len__(),self.candidate[i])
                self.freqItemset.insert(self.freqItemset.__len__(),self.candidate[i])
        while curr_candidate.__len__() > 1:
            curr_freqItemset = []
            for i in range(len(curr_candidate) - 1):
                for j in range(i, len(curr_candidate)):
                    if self.is_valid(curr_candidate[i], curr_candidate[j]) == True:
                        c = curr_candidate[i][:]
                        c.insert(c.__len__(), curr_candidate[j][c.__len__() - 1])
                        self.candidate.insert(self.candidate.__len__(),c)
                        if self.transaction.get_support(c) >= self.transaction.min_record:
                            curr_freqItemset.insert(curr_freqItemset.__len__(), c)
            self.freqItemset.extend(curr_freqItemset)
            curr_candidate = curr_freqItemset
    def export_frequentItemset(self, output):
        f = open(output, 'w')
        for item in self.freqItemset:
            f.write(self.transaction.to_string(item))
        f.close()
    def export_candidateItemset(self,output):
        f = open(output, 'w')
        for item in self.candidate:
            f.write(self.transaction.to_string(item))
        f.close()
import sys
class Parameter:
    def __init__(self):
        for i in range(1,len(sys.argv)):
            if "inp=" in sys.argv[i]:
                self.input = str(sys.argv[i]).split("=")[1]
            if "minsup=" in sys.argv[i]:
                self.min_sup = float(str(sys.argv[i]).split("=")[1])
            if "out_frq=" in sys.argv[i]:
                self.out_frq = str(sys.argv[i]).split("=")[1]
            if "out_all=" in sys.argv[i]:
                self.out_all = str(sys.argv[i]).split("=")[1]

import datetime
all_parameter = Parameter()
if not hasattr(all_parameter,'input'):
    print('Khong co file input')
    exit()
if not hasattr(all_parameter,'min_sup'):
    all_parameter.min_sup = 0
start = datetime.datetime.now()
apr = Apriori(all_parameter.min_sup ,all_parameter.input)
apr.cadidate_gen()
if hasattr(all_parameter,'out_frq'):
    apr.export_frequentItemset(all_parameter.out_frq)
if hasattr(all_parameter, 'out_all'):
    apr.export_candidateItemset(all_parameter.out_all)
finish = datetime.datetime.now()
time = finish - start
print('Time running : ' + str(time) + ' microsecond')
print('Frequent Itemset : ' + str(apr.freqItemset.__len__()))
print('Candidate Itemset : ' + str(apr.candidate.__len__()))