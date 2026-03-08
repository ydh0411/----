#include<iostream>
#include<vector>
#include<functional>
#include<stdexcept>
using namespace std;
class weightedDiGraph{//有向加权图的类
    public:
    struct Edge{
        int to;
        int weight;
        Edge(int t,int w){
            this->to=t;
            this->weight=w;
        }
    };
    private:
    vector<vector<Edge>>graph;//图的邻接表
    public:
    weightedDiGraph(int n){
        graph=vector<vector<Edge>>(n);//初始化图的邻接表,传入了图的顶点数量n
    }
    void addEdge(int from,int to,int weight){
        if(from<0||from>=graph.size()||to<0||to>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        graph[from].emplace_back(to,weight);//在邻接表中添加边,from是边的起点,to是边的终点,weight是边的权重,emplace_back函数会在vector的末尾添加一个新的Edge对象
    }
    void removeEdge(int from,int to){
        if(from<0||from>=graph.size()||to<0||to>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        for(auto it=graph[from].begin();it!=graph[from].end();++it){
            if(it->to==to){
                graph[from].erase(it);//在邻接表中删除边,from是边的起点,to是边的终点,erase函数会删除指定位置的元素
                return;
            }
        }
    }
    bool hasEdge(int from,int to){
        if(from<0||from>=graph.size()||to<0||to>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        for(const auto& edge:graph[from]){
            if(edge.to==to){
                return true;//检查图中是否存在边,from是边的起点,to是边的终点,如果在邻接表中找到了对应的边,就返回true
            }
        }
        return false;
    }
    int weight(int from,int to){
       if(from<0||from>=graph.size()||to<0||to>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        for(const auto& edge:graph[from]){
            if(edge.to==to){
                return edge.weight;//获取边的权重,from是边的起点,to是边的终点,如果在邻接表中找到了对应的边,就返回它的权重
            }
        }
        throw invalid_argument("No such edge");//如果在邻接表中没有找到对应的边,就抛出一个异常
    }
    //遍历，获取所有相邻节点及边的权重，复杂度 O(n)
    void traverse(const Graph&graph,int s,vector<bool>&visited){
        if(s<0||s>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        if(visited[s]){//如果当前顶点已经被访问过了,就直接返回,visited是一个布尔类型的数组,用来记录每个顶点是否被访问过了
            return;
        }
        visited[s]=true;//标记当前顶点已经被访问过了
        cont<<s<<" ";//输出当前顶点的编号
        for(const Graph::Edge&e:graph.nerighbors(s)){
            traverse(graph,e.to,visited);//递归地访问当前顶点的所有邻居,e.to是邻居的编号
        }
    }
    const vector<Edge>& getEdges(int vertex){
        if(vertex<0||vertex>=graph.size()){
            throw invalid_argument("Invalid vertex index");
        }
        return graph[vertex];//获取一个顶点的所有出边,vertex是顶点的索引,返回该顶点在邻接表中的边列表
    }
};
int main(){
    weightedDiGraph g(5);//创建一个有向加权图对象g,图中有5个顶点
    g.addEdge(0,1,10);//在图中添加边,从顶点0到顶点1,权重为10
    g.addEdge(0,2,5);//在图中添加边,从顶点0到顶点2,权重为5
    g.addEdge(1,2,2);//在图中添加边,从顶点1到顶点2,权重为2
    g.addEdge(1,3,1);//在图中添加边,从顶点1到顶点3,权重为1
    g.addEdge(2,1,3);//在图中添加边,从顶点2到顶点1,权重为3
    g.addEdge(2,3,9);//在图中添加边,从顶点2到顶点3,权重为9
    g.addEdge(2,4,2);//在图中添加边,从顶点2到顶点4,权重为2
    g.addEdge(3,4,4);//在图中添加边,从顶点3到顶点4,权重为4
    g.addEdge(4,3,6);//在图中添加边,从顶点4到顶点3,权重为6

    cout<<"Edges from vertex 0:"<<endl;
    for(const auto& edge:g.getEdges(0)){
        cout<<"To: "<<edge.to<<", Weight: "<<edge.weight<<endl;//输出从顶点0出发的所有边的信息
    }
    cout<<"Weight of edge from 0 to 1: "<<g.weight(0,1)<<endl;//输出从顶点0到顶点1的边的权重

    return 0;
}