# BFS算法框架
1. 梳理一下DFS/回溯算法的核心思想是递归遍历一颗多叉树，而多叉树的遍历又是由二叉树的遍历来的。
2. BFS的核心是遍历一张图，图的遍历又是多叉树加visited数组来避免死循环。为什么BFS更易于求解最短路径问题呢？因为DFS这样的遍历相当于要遍历整棵树的所有路径，而BFS这样的层序遍历相当于一层一层地遍历树的每一层，所以当我们找到目标节点时，我们就可以确定它是最短路径了。
3. BFS的代码框架如下：
```cpp
    int bfs(const Graph&graph,int s,int target){
        vector<bool>visited(graph.size(),false);//visited数组记录节点是否被访问过
        queue<int>q;//队列记录待访问的节点
        q.push(s);//将起始节点加入队列
        visited[s]=true;//标记起始节点为已访问
        int step=0;//记录当前的层数，也就是距离起始节点的距离
        while(!q.empty()){
            int sz=q.size();
            for(int i=0;i<sz;i++){
                int cur=q.front();
                q.pop( );//访问当前节点
                cout<<cur<<" ";//访问当前节点
                if(cur==target){
                    return step;//如果当前节点是目标节点，返回当前的层数
                }
                for(int neighbor:graph[cur]){
                    if(!visited[neighbor]){
                        q.push(neighbor);
                        visited[neighbor]=true;//标记邻居节点为已访问
                    }
                }
                
            }
            step++;//层数加一

        }
        return -1;//如果没有找到目标节点，返回-1
    }
```
## 题目一：滑动谜题
1. 各一个2*3的棋盘上有 5 个 1~5 的数字和一个 0 代表空格。每次移动，你可以选择 0 与一个相邻数字（上下左右）交换。最终当棋盘上数字顺序为 [[1,2,3],[4,5,0]] 时，游戏结束。给出一个初始状态 board，返回最少的移动次数，如果无法完成游戏则返回 -1 。
2. 思路：我们通过映射将二维棋盘转换为一维字符串来表示状态，然后使用BFS来遍历所有可能的状态，直到找到目标状态。
```cpp
#include<vectotr>
#include<string>
#include<queue>
#include<unordered_set>
class Solution{
    public:
    int slidingPuzzle(vector<vector<int>>&board){
        string start="";
        for(int i=0;i<board.size();i++){
            for(int j=0;j<board[0].size();j++){
                start+=to_string(board[i][j]);
            }
        }
        string target="123450";
        queue <string>q;
        unordered_set<string>visited;
        //
        q.push(strat);
        visited.insert(start);
        int step=0;
        while(!q.empty()){
            int sz=q.size();
            for(int i=0;i<sz;i++){
                string cur=q.front();
                q.pop();
                if(cur==target){
                    return step;
                }
                for(string next:getNextStates(cur)){
                    if(!visited.count(next)){
                        q.push(next);
                        visited.insert(next);

                    }
                }
            }
            step++;
        }
        return -1;
    }
    vector<string>getNextStates(string board){
        vector<vector<int>>mapping={{1,3},{0,2,4},{1,5},{0,4},{1,3,5},{2,4}};
        int idx=board.find('0');
        vector<string>nextStates;
        for(int neighbor:mapping[idx]){
            string next=board;
            swap(next[idx],next[neighbor]);
            nextStates.push_back(next);
        }
        return nextStates;

    }
    string swap(string str,int i,int j){
        char temp=str[i];
        str[i]=str[j];
        str[j]=temp;
        return str;
    }
}
```