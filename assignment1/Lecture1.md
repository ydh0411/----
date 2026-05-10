# Lecture 1: 图像分类与线性分类器

## 1. 图像分类的挑战

对于计算机来说，图像是一个像素值矩阵。每个像素通常是一个 `0-255` 的整数，表示亮度或颜色信息。彩色图像（RGB）每个像素包含三个通道，因此图像可表示为一个高维向量。

**示例：** 一张 $32 \times 32$ 的 RGB 图像 → $32 \times 32 \times 3 = 3072$ 维向量。

图像分类面临的主要挑战：

| 挑战 | 说明 |
|------|------|
| Viewpoint variation（视角变化） | 同一对象在不同视角下可能完全不同 |
| Scale variation（尺度变化） | 同一对象在不同距离下大小不同 |
| Occlusion（遮挡） | 对象可能被其他物体部分遮挡 |
| Deformation（形变） | 同一对象可能具有不同的形状或姿态 |
| Intra-class variation（类内差异） | 同一类别的对象外观可能差异很大 |
| Background clutter（背景杂乱） | 对象可能出现在复杂背景中 |
| Low resolution（低分辨率） | 图像可能分辨率较低 |

---

## 2. 数据驱动方法 (Data-Driven Approach)

硬编码规则无法应对复杂的图像分类，因此采用**数据驱动**的方式从大量数据中学习。

**基本流程：**

1. **收集数据**：收集包含图像和对应标签的数据集
2. **训练分类器**：用机器学习方法（如线性分类器）训练模型
3. **预测与评估**：用训练好的分类器对新图像进行分类

> 线性分类器的核心思想：将图像表示为高维向量，寻找超平面在空间中分离不同类别。

---

## 3. 评分函数 (Score Function)

线性分类器的核心是评分函数，将输入像素映射为各类别的置信度分数：

$$ f(x_i, W, b) = W x_i + b $$

| 符号 | 含义 | 维度 |
|------|------|------|
| $x_i$ | 第 $i$ 张图像展平后的列向量 | $D$（如 3072） |
| $W$ | 权重矩阵 | $K \times D$（$K$ 为类别数） |
| $b$ | 偏置向量 | $K$ |
| 输出 | 各类别得分 | $K$ 维向量 |

---

## 4. 最近邻分类器 (Nearest Neighbor)

将测试图片与训练集中每张图片比较，找到距离最近的图片，将其标签作为预测结果。

- **优点**：逻辑简单，训练只需记录数据（$O(1)$）
- **缺点**：测试阶段计算量极大，需与每张训练图计算距离

### 距离度量

**L1 距离（曼哈顿距离）：**

$$ d_1(x, y) = \sum_{i} |x_i - y_i| $$

**L2 距离（欧氏距离）：**

$$ d_2(x, y) = \sqrt{\sum_{i} (x_i - y_i)^2} $$

> L2 距离在需要考虑维度间组合关系时更合适，但计算量因平方和开方操作略大于 L1。

### 代码实现（L1 距离）

```python
import numpy as np

Xtr, Ytr, Xte, Yte = load_CIFAR10('cifar-10-batches-py')
# Xtr: 50000x32x32x3, Ytr: 50000, Xte: 10000x32x32x3, Yte: 10000

Xte_rows = Xte.reshape(Xte.shape[0], 32 * 32 * 3)  # 10000 x 3072
Xtr_rows = Xtr.reshape(Xtr.shape[0], 32 * 32 * 3)  # 50000 x 3072

class NearestNeighbor:
    def train(self, X, Y):
        self.Xtr = X
        self.Ytr = Y

    def predict(self, X):
        Yte_predict = np.zeros(X.shape[0], dtype=self.Ytr.dtype)
        for i in range(X.shape[0]):
            distances = np.sum(np.abs(self.Xtr - X[i, :]), axis=1)
            min_index = np.argmin(distances)
            Yte_predict[i] = self.Ytr[min_index]
        return Yte_predict

nn = NearestNeighbor()
nn.train(Xtr_rows, Ytr)
Yte_predict = nn.predict(Xte_rows)
print('accuracy: %f' % (np.mean(Yte_predict == Yte)))
```

> **关键特性**：`predict` 中实时计算距离 → "训练快，测试慢"。工程中期望相反——训练可以慢，但推理必须快。

---

## 5. K-最近邻分类器 (k-NN)

不只看最近的一个邻居，而是找距离最近的 **$k$** 个图像，通过**多数表决**确定最终类别。

### $k$ 值的选择

| $k$ 值 | 效果 |
|--------|------|
| 较小（如 $k=1$） | 贴合训练数据，易拟合噪声 → **过拟合** |
| 较大 | 决策边界更平滑，抗噪；过大则 → **欠拟合** |

通常通过**验证集**或**交叉验证**选择最优 $k$。

### 方法一：验证集调参

```python
Xval_rows = Xtr_rows[:1000, :]   # 前 1000 个作为验证集
Yval = Ytr[:1000]
Xtr_rows = Xtr_rows[1000:, :]    # 剩余作为训练集
Ytr = Ytr[1000:]

for k in [1, 3, 5, 8, 10, 20, 30, 50, 70, 100]:
    nn = NearestNeighbor()
    nn.train(Xtr_rows, Ytr)
    Yval_predict = nn.predict(Xval_rows)
    acc = np.mean(Yval_predict == Yval)
    print('k=%d, accuracy: %f' % (k, acc))
```

### 方法二：交叉验证 (Cross-Validation)

将训练集划分为多个 fold，轮流将每个 fold 作为验证集，其余作为训练集，取平均准确率最高的 $k$。

### 实际应用要点

1. **归一化**：将特征缩放到统一范围
2. **降维**：使用 PCA 等方法减少计算量
3. **在验证集上调参**：选择合适的 $k$ 值

**总结**：KNN 简单直观，但每次预测需计算所有训练样本的距离，计算量大。

---

## 6. 线性分类器 (Linear Classifier)

评分函数 $f(x_i, W, b) = W x_i + b$，从三个角度理解：

### 角度一：分类器组合

$W$ 的每一行对应一个类别的分类器，每一列对应一个像素位置。$Wx$ 得到各类别得分，最高分为预测类别。

### 角度二：模板匹配

$W$ 的每一行是该类别的"原型图像"（模板），$Wx$ 的内积衡量输入与各模板的匹配程度。匹配度最高的即为预测类别。

> 模板通过训练学习得到：训练中调整 $W$ 使正确类别得分更高、错误类别得分更低。

### 角度三：高维空间中的超平面

图像是高维空间中的点，线性分类器寻找超平面分割不同类别。$W$ 定义了超平面的位置和方向。

### 与 KNN 的区别

| KNN | 线性分类器 |
|-----|-----------|
| 基于距离 | 基于线性组合 $Wx$ |
| 需计算所有训练样本距离 | 只需一次矩阵乘法 |
| 无训练过程 | 需要训练学习 $W$ |

### 偏置项与权重的合并

将偏置合并到权重矩阵中以简化计算：在输入 $x$ 末尾追加常数 1，在 $W$ 最后一列追加偏置列向量。这样 $Wx + b$ 等价于 $W' x'$。

### 图像预处理

1. **归一化**：将像素值缩放到统一范围（如 $[0, 1]$ 或 $[-1, 1]$）
2. **零中心化**：减去训练集均值，使数据中心化，有助于梯度下降收敛

---

# Lecture 2: 损失函数与最优化

## 1. 损失函数 (Loss Function)

损失函数衡量预测结果与真实标签的差距。对于 $N$ 个样本 $\{(x_i, y_i)\}_{i=1}^N$：

$$ L = \frac{1}{N} \sum_{i=1}^{N} L_i(f(x_i, W), y_i) $$

---

## 2. 多类 SVM 损失 (Multiclass SVM Loss)

对于样本 $(x_i, y_i)$，得分向量 $s = W x_i$，SVM 损失为：

$$ L_i = \sum_{j \neq y_i} \max(0, s_j - s_{y_i} + \Delta) $$

- $s_{y_i}$：正确类别的得分
- $s_j$：其他类别的得分
- $\Delta$：边距（margin），正确类比其他类得分至少高出 $\Delta$（通常取 $\Delta = 1$）

> $\Delta$ 通常取 1 即可，因为 $W$ 的缩放能等价改变 margin 效果。

**变体：**
- **Hinge Loss**：$\max(0, -)$
- **Squared Hinge Loss**：$\max(0, -)^2$，对违反边距惩罚更重

### 代码实现（非向量化）

```python
def svm_loss_naive(x, y, W, delta=1.0):
    """
    x: 单张图像展平向量, shape (D,)
    y: 真实标签（整数索引）
    W: 权重矩阵, shape (D, C)
    返回: 该样本的 SVM loss
    """
    scores = x.dot(W)                     # shape (C,)
    correct_class_score = scores[y]
    num_classes = W.shape[1]
    loss_i = 0.0
    for j in range(num_classes):
        if j == y:
            continue
        loss_i += max(0, scores[j] - correct_class_score + delta)
    return loss_i
```

---

## 3. 正则化损失 (Regularization Loss)

### 为什么需要正则化？

若模型过于复杂，会过拟合训练数据，在测试集上表现不佳。正则化惩罚大权重，鼓励模型学习更简单的模式。

**示例：** 输入 $x = [1, 1, 1, 1]$，两个权重向量：
- $w_1 = [1, 0, 0, 0]$ — 只依赖一个特征，易受噪声影响
- $w_2 = [0.25, 0.25, 0.25, 0.25]$ — 分散权重，泛化能力更强

两者内积相同，但 $w_2$ 更鲁棒。

### 常见正则化

| 类型 | 公式 | 效果 |
|------|------|------|
| L2 正则化 | $R(W) = \sum_k \sum_l W_{k,l}^2$ | 惩罚大权重，鼓励权重分散 |
| L1 正则化 | $R(W) = \sum_k \sum_l \|W_{k,l}\|$ | 产生稀疏权重，有助于特征选择 |

正则化强度由超参数 $\lambda$ 控制：

$$ L = \frac{1}{N} \sum_{i=1}^{N} L_i + \lambda R(W) $$

---

## 4. SVM 损失完整表达式

$$ L = \frac{1}{N} \sum_{i=1}^{N} \sum_{j \neq y_i} \max(0, s_j - s_{y_i} + \Delta) + \lambda \sum_k \sum_l W_{k,l}^2 $$

其中 $s = f(x_i, W) = W x_i$（省略偏置项 $b$）。

---

## 5. Softmax 损失 (Cross-Entropy Loss)

将输出分数转换为概率分布，计算预测与真实类别的交叉熵损失。

### 步骤一：Softmax 函数（得分 → 概率）

$$ P(Y = k \mid X = x_i) = \frac{e^{s_k}}{\sum_j e^{s_j}} $$

### 步骤二：交叉熵损失

$$ L_i = -\log P(Y = y_i \mid X = x_i) = -\log\left(\frac{e^{s_{y_i}}}{\sum_j e^{s_j}}\right) $$

目标是使正确类别的概率尽可能接近 1，等价于最小化负对数似然。

### 完整表达式（含正则化）

$$ L = \frac{1}{N} \sum_{i=1}^{N} -\log\left(\frac{e^{s_{y_i}}}{\sum_j e^{s_j}}\right) + \lambda \sum_k \sum_l W_{k,l}^2 $$

### 数值稳定性

为防止指数溢出，减去最大得分（不改变概率结果）：

$$ \frac{e^{s_k}}{\sum_j e^{s_j}} = \frac{e^{s_k - C}}{\sum_j e^{s_j - C}}, \quad C = \max_j(s_j) $$

---

## 6. SVM vs Softmax 对比

| 特性 | SVM (Hinge Loss) | Softmax (Cross-Entropy) |
|------|-------------------|--------------------------|
| 输出含义 | 得分，无概率意义 | 类别概率分布 |
| 优化目标 | 正确类比其他类高出一个 margin | 正确类概率尽可能接近 1 |
| 对分数敏感度 | 满足 margin 后不再关心 | 始终追求更高正确类概率 |
| 适用场景 | 线性可分数据 | 多类分类，尤其类别有重叠时 |

---

## 7. 最优化 (Optimization)

目标：找到使损失函数最小化的 $W$。

### 7.1 随机搜索

在参数空间随机采样 $W$，保留损失最小的。简单但高维空间中效率极低。

### 7.2 梯度下降 (Gradient Descent)

沿梯度**反方向**更新参数（梯度指向上升最快方向，最小化需沿反方向）：

$$ W \leftarrow W - \eta \cdot \nabla_W L $$

- $\eta$：学习率，控制更新步长

| 变体 | 每次使用数据量 |
|------|--------------|
| 批量 GD (Batch) | 全部训练数据 |
| 随机 GD (SGD) | 1 个样本 |
| 小批量 GD (Mini-batch) | 一小批数据（**最常用**） |

### 7.3 数值梯度 (Numerical Gradient)

通过中心差分公式近似计算梯度：

$$ f'(x) \approx \frac{f(x + h) - f(x - h)}{2h} $$

- 优点：简单、不易出错
- 缺点：每个参数需两次前向计算，效率极低
- 用途：**验证解析梯度的正确性**（梯度检查）

### 7.4 解析梯度 (Analytical Gradient)

直接求导得到梯度的解析表达式，效率高，是实际训练中使用的方法。

**SVM 损失的梯度（对 $W$）：**

$$ \nabla_{W} L_i = \sum_{j \neq y_i} \mathbb{1}[s_j - s_{y_i} + \Delta > 0] \cdot (\text{对 } w_j \text{ 加 } x_i, \text{ 对 } w_{y_i} \text{ 减 } x_i) $$

**Softmax 损失的梯度（对 $W$）：**

$$ \nabla_{W} L_i = x_i \cdot \left( P(Y = k \mid X = x_i) - \mathbb{1}[k = y_i] \right)_{k=1}^{K} $$

即梯度 = (预测概率 - one-hot 真实分布) 与输入 $x_i$ 的外积。

---

## 8. 训练技巧

### 8.1 学习率 (Learning Rate)

| 过大 | 过小 |
|------|------|
| 训练发散，损失震荡甚至越来越大 | 收敛过慢，可能陷入局部最优 |

### 8.2 动量 (Momentum)

累积历史梯度，保持优化惯性：

$$ v \leftarrow \mu \cdot v - \eta \cdot \nabla_W L $$
$$ W \leftarrow W + v $$

- $\mu$：动量系数（通常 0.5 ~ 0.99）
- 效果：加速收敛，帮助跳出局部最优/鞍点

### 8.3 自适应学习率方法

为每个参数独立调整学习率：

| 方法 | 特点 |
|------|------|
| AdaGrad | 累计历史梯度平方和，学习率随训练递减 |
| RMSProp | 指数移动平均替代累加，避免过早衰减 |
| **Adam** | 结合动量和自适应学习率，**最常用** |

**Adam 更新公式：**

$$ m_t = \beta_1 m_{t-1} + (1 - \beta_1) \nabla_W L $$
$$ v_t = \beta_2 v_{t-1} + (1 - \beta_2) (\nabla_W L)^2 $$
$$ \hat{m}_t = \frac{m_t}{1 - \beta_1^t}, \quad \hat{v}_t = \frac{v_t}{1 - \beta_2^t} $$
$$ W \leftarrow W - \eta \cdot \frac{\hat{m}_t}{\sqrt{\hat{v}_t} + \epsilon} $$

典型取值：$\beta_1 = 0.9$，$\beta_2 = 0.999$，$\epsilon = 10^{-8}$。

### 8.4 小批量梯度下降

每次使用一小批数据（batch）计算梯度并更新参数。结合了批量 GD 的稳定性和 SGD 的效率。典型 batch size：32、64、128、256。

### 8.5 学习率衰减 (Learning Rate Decay)

训练后期逐步降低学习率，帮助精细收敛：

| 策略 | 公式 |
|------|------|
| 阶梯衰减 (Step) | 每隔固定 epoch 乘以因子（如 0.1） |
| 指数衰减 (Exponential) | $\eta = \eta_0 \cdot e^{-kt}$ |
| 时间衰减 (Time) | $\eta = \frac{\eta_0}{1 + kt}$ |
| 余弦衰减 (Cosine) | $\eta = \eta_{\min} + \frac{1}{2}(\eta_0 - \eta_{\min})(1 + \cos(\frac{t}{T}\pi))$ |

### 8.6 早停 (Early Stopping)

监控验证集性能，若连续 patience 轮无提升则提前停止。有效防止过拟合，节省训练时间。

### 8.7 正则化手段汇总

| 方法 | 说明 |
|------|------|
| L1 正则化 (Lasso) | 产生稀疏权重，特征选择 |
| L2 正则化 (Ridge/Weight Decay) | 惩罚大权重，使权重分布均匀 |
| Dropout | 训练时随机丢弃部分神经元，隐式集成 |
| 数据增强 (Data Augmentation) | 随机翻转、裁剪、颜色抖动等，**最有效** |

### 8.8 批量归一化 (Batch Normalization)

对每层激活值归一化到均值 0、方差 1，再通过可学习参数恢复表达能力：

$$ \hat{x} = \frac{x - \mu_{\text{batch}}}{\sqrt{\sigma^2_{\text{batch}} + \epsilon}} $$
$$ y = \gamma \hat{x} + \beta $$

效果：加速收敛、减少对初始化的敏感性、自带一定正则化效果。

### 8.9 超参数调优

| 方法 | 说明 |
|------|------|
| 网格搜索 (Grid Search) | 遍历所有预定义组合，维度高时计算量爆炸 |
| 随机搜索 (Random Search) | 随机采样，高维空间比网格搜索更高效 |
| 贝叶斯优化 | 利用历史结果指导后续搜索，更智能 |

> **实践建议**：$\Delta$ 通常固定为 1；$\lambda$ 在 log 尺度搜索（$10^{-4}, 10^{-3}, 10^{-2}, 10^{-1}, 1$）；学习率是最重要的超参数，优先调优。

---

# Lecture 3: 神经网络与反向传播

## 1. 反向传播 (Backpropagation)

利用链式法则，从输出层向输入层逐层传播误差，高效计算每个参数的梯度。

### 基本步骤

1. **前向传播**：计算网络输出和损失
2. **计算输出层梯度**：损失函数对输出层输入的偏导数，表示输出层输入应如何调整以减少损失
3. **逐层反向传播**：利用链式法则，从输出层向前逐层计算梯度
4. **更新参数**：用梯度更新权重和偏置

### 示例：$f(x, y, z) = (x + y) \times z$

**前向传播：**
- $a = x + y$
- $f = a \times z$

**输出层梯度：**
- $\frac{\partial f}{\partial a} = z$
- $\frac{\partial f}{\partial z} = a$

**反向传播（链式法则）：**
- $\frac{\partial f}{\partial x} = \frac{\partial f}{\partial a} \cdot \frac{\partial a}{\partial x} = z \cdot 1 = z$
- $\frac{\partial f}{\partial y} = \frac{\partial f}{\partial a} \cdot \frac{\partial a}{\partial y} = z \cdot 1 = z$
- $\frac{\partial f}{\partial z} = a$

**参数更新：**
```python
x -= learning_rate * df_dx
y -= learning_rate * df_dy
z -= learning_rate * df_dz
```

### 门单元 (Gate)

反向传播的基本构建块：

| 门类型 | 前向 | 反向传播（局部梯度） |
|--------|------|---------------------|
| 加法门 | $a = x + y$ | $\frac{\partial a}{\partial x} = 1, \frac{\partial a}{\partial y} = 1$ |
| 乘法门 | $f = a \times z$ | $\frac{\partial f}{\partial a} = z, \frac{\partial f}{\partial z} = a$ |
| max 门 | $f = \max(a, z)$ | 较大者梯度为 1，较小者为 0 |

核心思想：构建计算图，通过局部梯度的链式乘积得到每个参数的最终梯度。

### 向量化反向传播

实际网络中，操作是向量化的（如 $f = Wx + b$）。直接计算 $x$、$W$、$b$ 的整个梯度矩阵，无需逐元素计算。

**雅可比矩阵 (Jacobian)**：对于函数 $f: \mathbb{R}^n \to \mathbb{R}^m$，雅可比矩阵是 $m \times n$ 的偏导数矩阵。通过雅可比矩阵将链式法则应用到向量化操作中。

---

## 2. 激活函数

### Sigmoid

$$ \sigma(x) = \frac{1}{1 + e^{-x}} $$

- 输出范围 $(0, 1)$，具有概率意义
- 缺点：输入较大或较小时梯度接近 0，导致**梯度消失**

### ReLU

$$ \text{ReLU}(x) = \max(0, x) $$

- 计算简单，有效缓解梯度消失
- 缺点：$x < 0$ 时梯度为 0，可能导致神经元"死亡"

### 选择建议

| 层类型 | 推荐激活函数 |
|--------|------------|
| 隐藏层 | ReLU（最常用） |
| 二分类输出层 | Sigmoid |
| 多分类输出层 | Softmax |

---

## 3. 神经网络结构

由多个层次组成，每层包含多个神经元。每个神经元接收输入 → 加权求和 → 激活函数 → 输出。

| 层 | 作用 |
|----|------|
| 输入层 | 接收原始数据（如图像像素） |
| 隐藏层 | 通过非线性变换提取特征（可多层 → 深度网络） |
| 输出层 | 产生最终预测（如分类概率） |

### 全连接层 (Fully Connected Layer)

每个神经元与上一层所有神经元相连。

**参数数量计算**：$N_{in} \times N_{out} + N_{out}$（权重 + 偏置）

例如：输入 100 维，输出 10 维 → $100 \times 10 + 10 = 1010$ 个参数。

**本质理解**：全连接层是一族函数，每个函数对应特定的 $W$ 和 $b$。通过多层堆叠 + 非线性激活来拟合复杂函数，学习数据的层次化特征。

---

# Lecture 4: 卷积神经网络 (CNN)

## 1. 为什么需要 CNN？

**全连接网络的问题**：将 $32 \times 32 \times 3$ 的图像展平为 3072 维向量 → 丢失空间结构信息。

**CNN 的优势**：保留图像的空间结构，通过卷积操作提取局部特征。

---

## 2. CNN 核心组件

### 整体结构

```
[卷积层 → 激活函数 → 池化层]*N → 全连接层 → 输出
```

### 各组件作用

| 组件 | 作用 |
|------|------|
| 卷积层 (Conv) | 局部连接 + 权重共享，提取空间特征 |
| 激活函数 (Activation) | 引入非线性（如 ReLU） |
| 池化层 (Pooling) | 下采样，减少空间尺寸和计算量 |
| 批量归一化 (BN) | 稳定训练，加速收敛 |
| 全连接层 (FC) | 将特征映射到最终输出 |

### 关键概念

- **局部连接**：每个神经元只连接输入的一个局部区域
- **权重共享**：同一卷积核在整个输入上滑动，使用相同权重
- **感受野 (Receptive Field)**：局部连接的空间范围，随层数加深逐渐扩大

---

## 3. 输出尺寸计算

对于输入 $(W, H, D_{in})$，卷积核大小 $F \times F$，步长 $S$，填充 $P$，卷积核数量 $K$：

$$ W_{out} = \frac{W - F + 2P}{S} + 1 $$

$$ H_{out} = \frac{H - F + 2P}{S} + 1 $$

$$ D_{out} = K $$

---

## 4. 参数数量

| 层类型 | 参数数量 |
|--------|---------|
| 卷积层 | $(F \times F \times D_{in}) \times K + K$ |
| 池化层 | 0（无可学习参数） |
| 全连接层 | $N_{in} \times N_{out} + N_{out}$ |

---

## 5. 经典 CNN 架构

| 架构 | 年份 | 核心贡献 |
|------|------|---------|
| **LeNet-5** | 1998 | 最早的 CNN，用于手写数字识别 |
| **AlexNet** | 2012 | 更深网络 + ReLU，ImageNet 冠军，大幅提升性能 |
| **VGGNet** | 2014 | 堆叠小卷积核（$3 \times 3$），16/19 层深网络 |
| **ResNet** | 2015 | 残差连接，解决深层梯度消失，可达 152 层 |
| **DenseNet** | 2016 | 密集连接，每层与前面所有层相连，增强特征传递 |

---

## 6. 激活函数在 CNN 中的作用

激活函数引入非线性，使网络能学习和表示复杂函数关系。没有激活函数，多层线性变换等价于单层线性变换，限制了模型表达能力。
