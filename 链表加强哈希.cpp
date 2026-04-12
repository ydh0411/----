#include<iostream>
#include<vector>// 需要包含这个头文件才能使用 std::vector
#include<unordered_map>// 需要包含这个头文件才能使用 std::unordered_map
#include<string>// 需要包含这个头文件才能使用 std::string
using namespace std;
template <typename K, typename V>
struct Node{
    K key;
    V val;
    Node*next;
    Node*prev;
    Node(K key,V val):key(key),val(val),next(nullptr),prev(nullptr){}
};
template <typename K, typename V>
class MyLinkedHashMap{
    public:
    MyLinkedHashMap(){
        head=new Node<K,V>(K(),V());
        tail=new Node<K,V>(K(),V());
        head->next=tail;
        tail->prev=head;
    }
    ~MyLinkedHashMap(){
        Node<K,V>*current=head->next;
        while(current!=tail){
            Node<K,V>*temp=current->next;
            delete current;
            current=temp;
        }
        delete head;
        delete tail;
        }
        MyLinkedHashMap(const MyLinkedHashMap&)=delete;// 禁止拷贝构造函数
        MyLinkedHashMap& operator=(const MyLinkedHashMap&)=delete;// 禁止拷贝赋值运算符
        V get(K key){
            if(map.find(key)==map.end()){// key 不存在，返回默认值
                return V();
            }
            return map[key]->val;// 返回对应的值
        }
        void put(K key,V val){
            if(map.find(key)==map.end()){
                Node<K,V>*newnode=new Node<K,V>(key,val);
                addLast(newnode);
                map[key]=newnode;
                return;
            }
            map[key]->val=val;
        }
        void remove(K key){
            if(map.find(key)==map.end()){
                return;
            }
            Node<K,V>*node=map[key];
            map.erase(key);
            removeNode(node);
            delete node;
                }
        bool containsKey(K key){// 判断是否包含某个键
            return map.find(key)!=map.end();
        }
        vector<K>keys(){
            vector<K>keylist;
            Node<K,V>*current=head->next;
           for(;current!=tail;current=current->next){
            keylist.push_back(current->key);
           }
           return keylist;
        }
        size_t size(){
            return map.size();
        }
        private:
        Node<K,V>*head;
        Node<K,V>*tail;
        unordered_map<K,Node<K,V>*>map;
        void addLast(Node<K,V>*node){
            node->prev=tail->prev;
            node->next=tail;
            tail->prev->next=node;
            tail->prev=node;
        }
        void removeNode(Node<K,V>*node){
            node->prev->next=node->next;
            node->next->prev=node->prev;
            node->next=node->prev=nullptr;
        }
};
int main(){
    MyLinkedHashMap<int,string>linkedHashMap;
    linkedHashMap.put(1,"one");
    linkedHashMap.put(2,"two");
    linkedHashMap.put(3,"three");
    cout<<linkedHashMap.get(2)<<endl;// 输出 "two"
    linkedHashMap.remove(2);
    cout<<linkedHashMap.containsKey(2)<<endl;// 输出 0 (false)
    vector<int>keys=linkedHashMap.keys();
    for(int key:keys){
        cout<<key<<" ";// 输出 1 3
    }
    cout<<endl;
    cout<<linkedHashMap.size()<<endl;// 输出 2
    return 0;
}
