#include<iostream>
#include<queue>
using namespace std;
class TreeNode{
    public:
    int val;
    TreeNode*left;
    TreeNode*right;
    TreeNode(int val):val(val),left(nullptr),right(nullptr){}
};
void transverse(TreeNode*root){
    if(root==nullptr){
        return;
    }
    cout<<root->val<<" ";// 访问当前节点
    transverse(root->left);// 递归访问左子树
    transverse(root->right);// 递归访问右子树

}
void levelorder(TreeNode*root){
    if(root==nullptr){
        return;
    }
    queue<TreeNode*>q;
    q.push(root);
    int depth=1;
    while(!q.empty()){
        int sz=q.size();
        for(int i=0;i<sz;i++){
            TreeNode*cur=q.front();
            q.pop();
            cout<<cur->val<<" ";// 访问当前节点
            if(cur->left!=nullptr){
                q.push(cur->left);
            }
            if(cur->right!=nullptr){
                q.push(cur->right);
            }
        }
        depth++;
    }
}
