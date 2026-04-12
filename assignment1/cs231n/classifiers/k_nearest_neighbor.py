from builtins import range
from builtins import object
import numpy as np
from past.builtins import xrange


class KNearestNeighbor(object):
    """ a kNN classifier with L2 distance """

    def __init__(self):
        pass

    def train(self, X, y):
        """
        Train the classifier. For k-nearest neighbors this is just
        memorizing the training data.

        Inputs:
        - X: A numpy array of shape (num_train, D) containing the training data
          consisting of num_train samples each of dimension D.
        - y: A numpy array of shape (N,) containing the training labels, where
             y[i] is the label for X[i].
        """
        self.X_train = X
        self.y_train = y

    def predict(self, X, k=1, num_loops=0):
        """
        Predict labels for test data using this classifier.

        Inputs:
        - X: A numpy array of shape (num_test, D) containing test data consisting
             of num_test samples each of dimension D.
        - k: The number of nearest neighbors that vote for the predicted labels.
        - num_loops: Determines which implementation to use to compute distances
          between training points and testing points.

        Returns:
        - y: A numpy array of shape (num_test,) containing predicted labels for the
          test data, where y[i] is the predicted label for the test point X[i].
        """
        if num_loops == 0:
            dists = self.compute_distances_no_loops(X)
        elif num_loops == 1:
            dists = self.compute_distances_one_loop(X)
        elif num_loops == 2:
            dists = self.compute_distances_two_loops(X)
        else:
            raise ValueError("Invalid value %d for num_loops" % num_loops)

        return self.predict_labels(dists, k=k)

    def compute_distances_two_loops(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using a nested loop over both the training data and the
        test data.

        Inputs:
        - X: A numpy array of shape (num_test, D) containing test data.

        Returns:
        - dists: A numpy array of shape (num_test, num_train) where dists[i, j]
          is the Euclidean distance between the ith test point and the jth training
          point.
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        for i in range(num_test):
            for j in range(num_train):
                # two_loops: 外层遍历 test，内层遍历 train。
                # 每次只计算一对样本的距离，逻辑最直观，但 Python 循环开销最大。
                # X[i] / self.X_train[j] shape 都是 (D,)
                diff = X[i] - self.X_train[j]
                dists[i, j] = np.sqrt(np.sum(diff ** 2))
                
        return dists

    def compute_distances_one_loop(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using a single loop over the test data.

        Input / Output: Same as compute_distances_two_loops
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        for i in range(num_test):
            # one_loop: 仅对 test 做循环；每次把一个 test 样本与所有 train 一次性做广播计算。
            # X[i] shape: (D,), self.X_train shape: (num_train, D)
            # diff shape: (num_train, D)
            diff = X[i] - self.X_train
            # axis=1 按特征维求和，得到该 test 到所有 train 的距离向量。
            dists[i, :] = np.sqrt(np.sum(diff ** 2, axis=1))
        return dists

    def compute_distances_no_loops(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using no explicit loops.

        Input / Output: Same as compute_distances_two_loops
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        # no_loops: 不写任何显式 for 循环，完全交给矩阵运算。
        # 利用恒等式 ||a-b||^2 = ||a||^2 + ||b||^2 - 2a·b
        # 目标一次性得到 (num_test, num_train) 的距离矩阵。

        # 每个 test 向量的平方和，shape: (num_test, 1)
        test_sq = np.sum(X ** 2, axis=1, keepdims=True)

        # 每个 train 向量的平方和，shape: (1, num_train)
        train_sq = np.sum(self.X_train ** 2, axis=1, keepdims=True).T

        # test 与 train 的两两内积，shape: (num_test, num_train)
        cross_term = X @ self.X_train.T

        # 广播相加后得到两两平方距离，再开根号得到 L2 距离。
        # 数值上可能出现极小负数（浮点误差），用 maximum 截断到 0 防止 sqrt 报 nan。
        dists_sq = test_sq + train_sq - 2 * cross_term
        dists = np.sqrt(np.maximum(dists_sq, 0.0))
        return dists

    def predict_labels(self, dists, k=1):
        """
        Given a matrix of distances between test points and training points,
        predict a label for each test point.

        Inputs:
        - dists: A numpy array of shape (num_test, num_train) where dists[i, j]
          gives the distance betwen the ith test point and the jth training point.

        Returns:
        - y: A numpy array of shape (num_test,) containing predicted labels for the
          test data, where y[i] is the predicted label for the test point X[i].
        """
        num_test = dists.shape[0]
        y_pred = np.zeros(num_test)
        for i in range(num_test):
            # A list of length k storing the labels of the k nearest neighbors to
            # the ith test point.
            closest_y = []
            # 1) 对第 i 个测试样本的所有训练距离排序，得到从近到远的训练样本索引。
            sorted_idx = np.argsort(dists[i, :])

            # 2) 取最近的 k 个训练样本索引。
            k_nearest_idx = sorted_idx[:k]

            # 3) 查出这 k 个邻居的标签，作为投票候选。
            closest_y = self.y_train[k_nearest_idx]

            # 4) 统计每个标签出现次数。
            # np.bincount 要求输入是非负整数标签，cs231n 的 CIFAR-10 标签满足该条件。
            vote_count = np.bincount(closest_y)

            # 5) 取票数最多的标签。
            # 若平票，np.argmax 返回第一个最大值索引 -> 自动选择较小标签，满足题目要求。
            y_pred[i] = np.argmax(vote_count)


            #########################################################################
            # TODO:                                                                 #
            # Now that you have found the labels of the k nearest neighbors, you    #
            # need to find the most common label in the list closest_y of labels.   #
            # Store this label in y_pred[i]. Break ties by choosing the smaller     #
            # label.                                                                #
            #########################################################################


        # 保持与标签数组类型一致（可选，但能减少后续比较时的类型噪音）。
        return y_pred.astype(self.y_train.dtype)
