
# coding: utf-8

#     Tiêu đề : Bài tập cá nhân 1
#     Họ tên  : Nguyễn Văn Hiếu
#     MSSV    : 1412166

# ## Sử dụng phương pháp bình phương tối thiểu để tìm đường thẳng có độ fit tốt nhất
# 1. Phát sinh tập điểm 2D D = {(xi,yi)|i = 1,2,...,n}
# 2. Vẽ các điểm đó trên hệ tọa độ 2D

# In[29]:

get_ipython().magic(u'matplotlib inline')
import numpy as np
import matplotlib.pyplot as plt
rang = 20
minusRange = -20
n = input("Nhập số điểm cần phát sinh : ")
a = np.random.randint(-20,20,int(n))
b = np.random.randint(-20,20,int(n))


# ## Tìm đường thẳng đi qua các điểm trên có độ fit tốt nhất
# 1. Trình bày bài toán:
#     Cho n điểm (x,y) có sẵn trên mặt phẳng 2D.
#     Tìm đường thẳng tạo độ fit tốt nhất. Đường thẳng có độ fit tốt nhất là đường thẳng có tổng bình phương chênh lệch tung độ y nhỏ nhất
# 2. Hướng giải quyết.
#     Đường thẳng có dạng : y = ax + b
#     Bài toán đưa ra là tìm các hệ số của đường thẳng sao cho tổng bình phương là nhỏ nhất.
#     Giá trị cần tính là : $$d = \sum\limits_{i=1}^{n}(y_i - (a + bx_i))^2$$
#     Để d đạt giá trị nhỏ nhất thì đạo hàm của d theo a và b = 0:$${\partial\over\partial a}{\sum\limits_{i=1}^n(y_i - (a + bx_i))}^2 = 0 \implies \widehat{a} = \bar{y} - \widehat{b}\bar{x}$$
#     Và $${\partial\over\partial b}{\sum\limits_{i=1}^n(y_i - (a + bx_i))}^2 = 0 \implies \widehat{b} = {{\sum\limits_{i = 1}^n(x_i - \bar{x})(y_i - \bar{y})}\over{\sum\limits_{i=1}^n(x_i - \bar{x})^2}}$$
#     Thay vào phương trình đường thẳng ban đầu ta có được đường thẳng cần tìm: $$y = \widehat{a}x + \widehat{b}$$

# In[34]:

plt.plot(a,b,'ro')
xtb = np.average(a)
ytb = np.average(b)

tu = 0;
mau = 0;
for index in range(int(n)):
    tu = (a[index] - xtb)*(b[index] - ytb);
    mau = (a[index] - xtb)**2
    
X = np.linspace(-20,20,1000)
Y = ytb + (tu/mau)*(X - xtb)

plt.plot(X,Y,'green')
plt.show()

