
# coding: utf-8

#     Tiêu đề : Bài tập cá nhân 01
#     Họ tên  : Nguyễn Văn Hiếu
#     MSSV    : 1412166

# ## Viết chương trình phân lớp Perceptron
# 1. Tạo $n_1$ điểm đỏ và $n_2$ điểm xanh
# 2. Vẽ các điểm đó lên mặt phẳng 2D

# In[12]:

get_ipython().magic(u'matplotlib inline')
import numpy as np
import matplotlib.pyplot as plt

n1 = int(input("Nhập số điểm xanh : "))
n2 = int(input("Nhập số điểm đỏ : "))
ran = 20;

b = np.random.randint(ran*(-1),ran,size=(n1,2));
r = np.random.randint(ran*(-1),ran,size=(n2,2));

plt.plot(b[:,0],b[:,1],'bo')
plt.plot(r[:,0],r[:,1],'ro')
plt.show();


# Hàm vẽ đường thẳng với vector w có dạng: $(w_0,w_1,w_2)$
# 
# Phương trình đường thẳng có dạng : $w_0 x + w_1 y + w_2 = 0$

# In[13]:

def DrawLine(w):
    X = np.linspace(-20,20,1000)
    Y = -(w.item(0)*X + w.item(2))/w.item(1);
    plt.plot(b[:,0],b[:,1],'bo')
    plt.plot(r[:,0],r[:,1],'ro')
    plt.plot(X,Y,'red')
    plt.ylim(-20,20)
    plt.show()


# Hàm kiểm tra có thể phân lớp không? Chưa cài đặt

# In[14]:

def isPossible(b,r):
    return True;


# ## Phát biểu bài toán:
# - Vẽ ra $n_1$ điểm màu đỏ (x,y) trong mặt phẳng 2D 
# - Vẽ ra $n_2$ điểm màu xanh (x,y) trong mặt phẳng 2D
# - Vẽ $n_1 + n_2$ điểm đó lên mặt phẳng 2D sử dụng $\color{red}{matplotlib}$
# - Sử dụng thuật toán phân lớp Perceptron để phân lớp 2 tập điểm đó.
# - Vẽ đường thẳng đó lên mặt phẳng tọa độ 2D.
# ## Hướng giải quyết vấn đề
# - Đường thẳng d dùng để phân lớp có dạng : ${W^T} P = 0$
#     + $W = \begin{bmatrix}w_0&w_1&w_2\end{bmatrix}$
#     + $P = \begin{bmatrix}x&y&1\end{bmatrix}$
# - Hàm tính dấu :
#     $$sign(P_i) = \begin{cases}
#         -1 \quad\text{if } {W^T} P_i \leq 0\\
#          1 \quad\text{if } {W^T} P_i > 0
#     \end{cases}$$
# - Xác định dấu của màu xanh và màu đỏ
#     $$sign(\color{red}{Đỏ}) = \begin{cases}
#         -1 \quad\text{if } \max(y_{\color{red}{đỏ}}) \leq \max(y_{\color{blue}{xanh}})\\
#          1 \quad\text{if } \max(y_{\color{red}{đỏ}}) > \max(y_{\color{blue}{xanh}})
#     \end{cases}$$
# - Sử dụng vòng lặp while - do:
#     - Trong mỗi vòng lặp duyệt $n_1 + n_2$ phần tử:
#         - Nếu có phần tử sai vị trí $P_i$ nếu
#             $sign(\color{red}{đỏ}/_\color{blue}{xanh})*sign(P_i) \leq 0$
#         - Đặt lại giá trị của W : 
#             $W = W + n*P_i*sign(\color{red}{đỏ}/_\color{blue}{xanh})$
# ## Một số vấn đề gặp phải
# - Khi tập $n_1 + n_2$ được random tự do không thể phân lớp thì thuật toán sẽ lặp vô hạn
# - Ý tưởng giải quyết:
#     - Kiểm tra khả năng có thể phân lớp được hay không của tập điểm trước khi vào vòng lặp bằng hàm $\color{red}{isPossible}$
#     - Đặt một giá trị ngưỡng $\color{red}{threshold}$ để chặn số lần lặp của vòng $\color{red}{while}$
#     - Ở đây em đặt ngưỡng là 10 vòng lặp $\color{red}{while}$. Nếu chưa thể phân chia thì in ra thông báo : Không thể phân chia

# In[15]:

yRedMax = np.max(r[:,1])
yBlueMax = np.max(b[:,1])
if(yRedMax < yBlueMax):
    signOfRed = -1;
    signOfBlue = 1;
else:
    signOfBlue = -1;
    signOfRed = 1;
    
b = np.insert(b,2,1,axis=1)
r = np.insert(r,2,1,axis=1)

plt.plot(b[:,0],b[:,1],'bo')
plt.plot(r[:,0],r[:,1],'ro')
plt.show();


w = np.matrix('1,1,0');
n = 0.2

isClassification = False;
threshold = 0;
while isClassification == False and threshold < 10:
    threshold = threshold + 1;
    isClassification = True
    for row in b:
        result = np.dot(row,np.matrix.transpose(w))
        if(result*signOfBlue <= 0):
            w = w + n*row*signOfBlue
            DrawLine(w)
            isClassification = False;
    for row in r:
        result = np.dot(row,np.matrix.transpose(w))
        if(result*signOfRed <= 0):
            w = w + n*row*signOfRed
            DrawLine(w)
            isClassification = False;
if isClassification: 
    DrawLine(w)
else:
    print("Khong the phan chia")
    

