#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 多叉树节点定义
class Node {
public:
    int val;
    vector<Node*> children;
    
    // 构造函数
    Node(int val) : val(val) {}
    
    // 析构函数 - 递归释放所有子节点
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
    
    // 添加子节点
    void addChild(Node* child) {
        if (child != nullptr) {
            children.push_back(child);
        }
    }
};

// 前序遍历（深度优先遍历）
void traverse(Node* root) {
    if (root == nullptr) {
        return;
    }
    cout << root->val << " ";  // 访问当前节点
    for (Node* child : root->children) {  // 修复：正确的范围for循环语法
        traverse(child);  // 递归访问子节点
    }
}

// 后序遍历
void postorderTraverse(Node* root) {
    if (root == nullptr) {
        return;
    }
    for (Node* child : root->children) {
        postorderTraverse(child);  // 先递归访问子节点
    }
    cout << root->val << " ";  // 最后访问当前节点
}

// 层序遍历（广度优先遍历）
void levelOrder(Node* root) {
    if (root == nullptr) {
        return;
    }
    queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* cur = q.front();
            q.pop();
            cout << cur->val << " ";
            for (Node* child : cur->children) {
                q.push(child);
            }
        }
    }
}

// 按层级打印（每层换行）
void levelOrderByLevel(Node* root) {
    if (root == nullptr) {
        return;
    }
    queue<Node*> q;
    q.push(root);
    int depth = 0;
    
    while (!q.empty()) {
        int sz = q.size();
        cout << "层级 " << depth << ": ";
        for (int i = 0; i < sz; i++) {
            Node* cur = q.front();
            q.pop();
            cout << cur->val << " ";
            for (Node* child : cur->children) {
                q.push(child);
            }
        }
        cout << endl;
        depth++;
    }
}

// 计算树的最大深度
int maxDepth(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    int maxChildDepth = 0;
    for (Node* child : root->children) {
        maxChildDepth = max(maxChildDepth, maxDepth(child));
    }
    return maxChildDepth + 1;
}

// 计算树的节点总数
int countNodes(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    int count = 1;  // 当前节点
    for (Node* child : root->children) {
        count += countNodes(child);
    }
    return count;
}

// 查找值为target的节点
Node* findNode(Node* root, int target) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->val == target) {
        return root;
    }
    for (Node* child : root->children) {
        Node* result = findNode(child, target);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

// 打印从根到叶子的所有路径
void printPaths(Node* root, vector<int>& path) {
    if (root == nullptr) {
        return;
    }
    
    path.push_back(root->val);
    
    // 如果是叶子节点，打印路径
    if (root->children.empty()) {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        // 否则继续遍历子节点
        for (Node* child : root->children) {
            printPaths(child, path);
        }
    }
    
    path.pop_back();  // 回溯
}

// 测试函数
int main() {
    // 构建示例多叉树
    //        1
    //      / | \
    //     2  3  4
    //    /|    / \
    //   5 6   7   8
    
    Node* root = new Node(1);
    Node* node2 = new Node(2);
    Node* node3 = new Node(3);
    Node* node4 = new Node(4);
    
    root->addChild(node2);
    root->addChild(node3);
    root->addChild(node4);
    
    node2->addChild(new Node(5));
    node2->addChild(new Node(6));
    
    node4->addChild(new Node(7));
    node4->addChild(new Node(8));
    
    cout << "前序遍历: ";
    traverse(root);
    cout << endl;
    
    cout << "后序遍历: ";
    postorderTraverse(root);
    cout << endl;
    
    cout << "层序遍历: ";
    levelOrder(root);
    cout << endl << endl;
    
    cout << "按层级打印:" << endl;
    levelOrderByLevel(root);
    cout << endl;
    
    cout << "树的最大深度: " << maxDepth(root) << endl;
    cout << "树的节点总数: " << countNodes(root) << endl << endl;
    
    cout << "从根到叶子的所有路径:" << endl;
    vector<int> path;
    printPaths(root, path);
    cout << endl;
    
    int target = 6;
    Node* found = findNode(root, target);
    if (found) {
        cout << "找到节点: " << found->val << endl;
    } else {
        cout << "未找到节点: " << target << endl;
    }
    
    // 清理内存
    delete root;
    
    return 0;
}