#include <iostream>
#include <vector>
using namespace std;

// 加权有向图的通用实现（邻接矩阵）
class WeightedDigraph {
public:
    // 存储相邻节点及边的权重
    struct Edge {
        int to;
        int weight;

        Edge(int to, int weight) : to(to), weight(weight) {}
    };

    WeightedDigraph(int n) {
        matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    }

    // 增，添加一条带权重的有向边，复杂度 O(1)
    void addEdge(int from, int to, int weight) {
        matrix[from][to] = weight;
    }
    void removeEdge(int from,int to){
        matrix[from][to] = 0;
    }
    // 查，检查是否存在边，复杂度 O(1)
    bool hasEdge(int from,int to){
        return matrix[from][to]!=0;
    }
    int weight(int from,int to){
        return matrix[from][to];
    }
    // 遍历，获取所有相邻节点及边的权重，复杂度 O(n)
    vector<Edge>neighbors(int vertex){
        vector<Edge>edges;
        for(int i=0;i<matrix.size();i++){
            if(matrix[vertex][i]!=0){
                edges.emplace_back(i,matrix[vertex][i]);
            }
        }
        return edges;
    }
    void traverse(){
        
    }

private:
    vector<vector<int>>matrix;
}; 
int main(){
    WeightedDigraph graph(5);
    graph.addEdge(0,1,10);
    graph.addEdge(0,2,20);
    graph.addEdge(1,3,30);
    graph.addEdge(2,3,40);
    graph.addEdge(3,4,50);

    cout<<"Edges from vertex 0:"<<endl;
    for(const auto& edge:graph.neighbors(0)){
        cout<<"To: "<<edge.to<<", Weight: "<<edge.weight<<endl;
    }
    return 0;
}